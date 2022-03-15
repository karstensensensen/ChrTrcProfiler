#pragma once

#include "ChrTrcCompileTimeString.h"

// file containing various macros for quickly setting up a ScopedTimer with relevant naming.

// ============ helpers ============

#define CT_HELPER_HELPER_HELPER_STR(str, mac) str##mac
#define CT_HELPER_HELPER_STR(str, mac) CT_HELPER_HELPER_HELPER_STR(str, mac)
#define CT_HELPER_STR(str) CT_HELPER_HELPER_STR(str, __LINE__)

// ============ macros ============

#ifndef CT_NO_PROFILING
#define CT_MAC(M) M
#else
#define CT_MAC(M)
#endif

#define CT_MEASURE_V(verbosity) CT_MAC(::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(__FUNCSIG__, ""))

#define CT_MEASURE_VC(verbosity, category) CT_MAC(::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(__FUNCSIG__, category))

#define CT_MEASURE_VNC(verbosity, name, category) CT_MAC(\
constexpr auto CT_HELPER_STR(STIMER_NAME) = ::ChrTrcProfiler::ChrTrcCompileTimeStr::CTStr(name) + " (" + ::ChrTrcProfiler::ChrTrcCompileTimeStr::CTStr(__FUNCTION__) + ")").data();\
::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(CT_HELPER_STR(STIMER_NAME).data(), category)\

#define CT_MEASURE_VN(verbosity, name) CT_MEASURE_VNC(verbosity, name, "")

#define CT_MEASURE() CT_MEASURE_V(0)

#define CT_MEASURE_C(category) CT_MEASURE_VC(0, category)

#define CT_MEASURE_NC(name, category) CT_MEASURE_VNC(0, name, category)

#define CT_MEASURE_N(name) CT_MEASURE_VN(0, name)
