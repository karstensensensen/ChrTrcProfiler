#pragma once

#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

namespace ChrTrcProfiler
{
	/// @brief structure containing a profiler datapoint for a single function run.
	/// size 64 bit: 32 bytes
	/// size 32 bit: 20 bytes
	struct profiledFunction
	{
		size_t name_hash;
		size_t cat_hash;
		size_t thrd_id;
		uint32_t func_start;
		uint32_t func_durr;
	};

	/// @brief a simple timer that auto ends when the destructor is called
	/// @tparam the verbosity of the current scoped timer.
	/// this controls wether the ScopedTimer will report its data to the profiler, depending on the current verbosity setting.
	template<size_t verbosity = 0>
	class ScopedTimer
	{
	public:
		/// @brief the timer is automaticly started on construction.
		/// 
		/// @param name the name the data should be associated with. This should always be passed as a string literal!
		/// @param cat the category the data should be associated with. This should always be passed as a string literal!
		/// 
		/// @attention
		/// any strings passed to this constructor should exclusively be string literals, meaning no strings should be generated at runtime!
		ScopedTimer(std::string_view name, std::string_view cat);
		/// @see stop()
		~ScopedTimer() { stop(); }

		/// @brief stops the timer and reports the data to an active CTProfiler session, if avaliable.
		void stop();

	protected:
		std::chrono::high_resolution_clock::time_point m_start;

		std::string_view m_name;
		std::string_view m_cat;
	};

	class CTProfiler
	{
	public:
		/// @brief begins a profiling session.
		/// 
		/// a profiling session will automaticly end once the buffer is as full as possible or if the timeout is hit.
		/// on a session end the data will be formatted to a json format and written to a file.
		/// to manually end a session, call endSession().
		/// 
		/// optionally, a verbosity setting can be set, where any reports with a verbosity than the current one, will be ignored.
		/// per default, the verbosity is at its max (0)
		/// 
		/// @param buff_size the size of the buffer to store the datapoints.
		/// @param file_out the file the json data will be written to. This can later be opened in a (chrome://tracing)[chrome://tracing] tab to view the data.
		/// @param append_date wether to add a time and date suffix to file_out.
		/// @param timeout how long the profiling session should run until it autoends. <= 0 no timeout
		static void beginSession(size_t buff_size, std::filesystem::path file_out,
			bool append_date = true, std::chrono::high_resolution_clock::duration timeout = std::chrono::high_resolution_clock::duration(-1));

		/// @brief ends a profiling session.  
		/// this will trigger a data parsing and file save, meaning the program might slow down when this is called.
		/// However, this slow down will not be recorded in the profiling data itself.
		static void endSession();

		/// @return wether a profiling session is currently running.
		static bool hasSession() { return m_is_profiling; }

		static void reportRecording(size_t verbosity, std::string_view name, std::string_view category,
			std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end);

		/// @brief set the verbosity of the logger.  
		/// 
		/// the verbosity of a session is checked whilst the functions are being timed.
		/// this means it matters wether a ScopedTimer with verbosity 2, is stopped before or after a setVerbosity(1) call.
		/// the start of a ScopedTimer does not matter here.
		/// 
		/// @param new_verb 
		static void setVerbosity(size_t new_verb) { m_verbosity = new_verb; }
		/// @brief returns the current profiling verbosity.
		/// @return 
		static size_t getVerbosity() { return m_verbosity; }

		static bool isValidVerb(size_t v) { return getVerbosity() == 0 ? true : v <= getVerbosity(); }
	
	protected:
		
		/// @brief thread that waits until the timeout is hit and then calls endSession
		static void endSThrd();
		static inline std::thread m_end_thrd;
		static inline std::mutex m_mutex;
		static inline std::condition_variable m_cv;

		static inline size_t m_verbosity = 0;

		static inline std::chrono::high_resolution_clock::duration m_timeout;

		static inline std::atomic<bool> m_is_profiling;

		static inline std::filesystem::path m_file_out;

		static inline std::hash<std::string_view> m_str_hasher;
		static inline std::hash<std::thread::id> m_thrd_id_hasher;
		static inline std::vector<profiledFunction> m_profiling_data;
		static inline std::unordered_map<size_t, std::string_view> m_names;
		static inline std::unordered_map<size_t, std::string_view> m_categories;
		
	};

}


#include "ChrTrcProfiler.ipp"
