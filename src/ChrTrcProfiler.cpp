#include "ChrTrcProfiler.h"

#include <iostream>
#include <cassert>

using namespace std::chrono;

namespace ChrTrcProfiler
{
	void CTProfiler::beginSession(size_t buff_size, std::filesystem::path file_out, bool append_date, std::chrono::high_resolution_clock::duration timeout)
	{
		// allocate profiling buffer

		m_profiling_data.resize(0);
		m_profiling_data.reserve(buff_size / sizeof(profiledFunction));
		
		if (append_date)
		{
			std::time_t now = system_clock::to_time_t(system_clock::now());
			std::tm* timepoint_now = std::localtime(&now);

			constexpr size_t do_size = 21;
			char date_out[do_size];

			strftime(date_out, do_size, "_%Y.%m.%d_%H-%M-%S", timepoint_now);

			m_file_out = file_out.replace_filename(file_out.stem().string() + date_out + file_out.extension().string());
		}

		m_is_profiling = true;

		// only begin timeout thread if timeout is set
		if (timeout.count() > 0)
		{
			m_timeout = timeout;
			
			m_end_thrd = std::thread(endSThrd);
		}
	}

	void CTProfiler::endSession()
	{
		if (m_is_profiling)
		{

			// make sure to fix any equal start times here.

			// start by stopping the endSThrd
			m_is_profiling = false;
			m_cv.notify_one();

			if (m_end_thrd.joinable())
				m_end_thrd.join();

			std::ofstream m_out(m_file_out);

			// write header
			m_out << "{\"traceEvents\":[";

			// write all data points
			for (size_t i = 0; i < m_profiling_data.size(); i++)
			{
				profiledFunction& data_point = m_profiling_data[i];
				m_out << "{\"pid\":0,\"ph\":\"X\",\"tid\":" << data_point.thrd_id << ",\"ts\":" << data_point.func_start << ",\"dur\":" << data_point.func_durr;

				m_out << ",\"cat\":\"" << m_categories[data_point.cat_hash] << '"';
				m_out << ",\"name\":\"" << m_names[data_point.name_hash] << '"';

				if (i + 1 == m_profiling_data.size())
					m_out << "}";
				else
					m_out << "},";
			}

			// write footer
			m_out << "]}";

			m_out.close();
		}
	}

	void CTProfiler::reportRecording(size_t verbosity, std::string_view name, std::string_view category, std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end)
	{
		if (isValidVerb(verbosity))
		{
			assert(m_is_profiling && "Can only record timing if a session is currently running!");

			// process the given data and put it into a profiledFunction structure
			profiledFunction data{ m_str_hasher(name), m_str_hasher(category), m_thrd_id_hasher(std::this_thread::get_id()),
				(uint32_t)duration_cast<microseconds>(start.time_since_epoch()).count(),
				(uint32_t)duration_cast<microseconds>(end.time_since_epoch() - start.time_since_epoch()).count() };

			if (m_names.find(data.name_hash) == m_names.end())
				m_names[data.name_hash] = name;

			if (m_categories.find(data.cat_hash) == m_categories.end())
				m_categories[data.cat_hash] = category;

			m_profiling_data.push_back(data);

			// end the session if the buffer is full
			if (m_profiling_data.size() == m_profiling_data.capacity())
				endSession();
		}
	}
	
	void CTProfiler::endSThrd()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			// should wait until the timeout is hit or the session is ended manually
			m_cv.wait_for(lock, m_timeout, []() { return !m_is_profiling.load(); });

			if(m_is_profiling)
				CTProfiler::endSession();
		}
	}
}
