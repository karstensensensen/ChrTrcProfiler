#pragma once

#include "ChrTrcProfiler.h"

namespace ChrTrcProfiler
{
	template<size_t verbosity>
	ScopedTimer<verbosity>::ScopedTimer(std::string_view name, std::string_view cat)
		: m_start(std::chrono::high_resolution_clock::now()), m_name(name), m_cat(cat)
	{}

	template<size_t verbosity>
	void ScopedTimer<verbosity>::stop()
	{
		if (CTProfiler::hasSession())
		{
			std::chrono::time_point end = std::chrono::high_resolution_clock::now();
			CTProfiler::reportRecording(verbosity, m_name, m_cat, m_start, end);
		}
	}
}
