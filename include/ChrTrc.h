#pragma once

#include "ChrTrcCompileTimeString.h"

// file containing various macros for quickly setting up a ScopedTimer with relevant naming.


// ============ helpers ============

#define CT_HELPER_HELPER_HELPER_STR(str, mac) str##mac
#define CT_HELPER_HELPER_STR(str, mac) CT_HELPER_HELPER_HELPER_STR(str, mac)
#define CT_HELPER_STR(str) CT_HELPER_HELPER_STR(str, __LINE__)

#ifdef WIN32
#define CT_FUNC_SIG __FUNCSIG__
#define CT_FUNC_NAME __FUNCTION__
#else
#define CT_FUNC_SIG __PRETTY_FUNCTION__
#define CT_FUNC_NAME __func__
#endif

#ifndef CT_NO_PROFILING
#define CT_GATE(M) M
#else
#define CT_GATE(M)
#endif

// ============ macros ============

#define CT_MEASURE_V(verbosity) CT_GATE(::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(CT_FUNC_SIG, ""))

#define CT_MEASURE_VC(verbosity, category) CT_GATE(::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(CT_FUNC_SIG, category))

#define CT_MEASURE_VNC(verbosity, name, category) CT_GATE(\
constexpr static auto CT_HELPER_STR(STIMER_NAME) = ::ChrTrcProfiler::ChrTrcCompileTimeStr::CTStr(name) + " (" + ::ChrTrcProfiler::ChrTrcCompileTimeStr::CTStr(CT_FUNC_NAME) + ")";\
::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(CT_HELPER_STR(STIMER_NAME).data(), category))\

#define CT_MEASURE_VN(verbosity, name) CT_MEASURE_VNC(verbosity, name, "")

#define CT_MEASURE() CT_MEASURE_V(0)

#define CT_MEASURE_C(category) CT_MEASURE_VC(0, category)

#define CT_MEASURE_NC(name, category) CT_MEASURE_VNC(0, name, category)

#define CT_MEASURE_N(name) CT_MEASURE_VN(0, name)
