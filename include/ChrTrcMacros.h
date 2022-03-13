#pragma once

// file containing various macros for quickly setting up a ScopedTimer with relevant naming.

// ============ helpers ============

#define CT_HELPER_HELPER_HELPER_STR(str, mac) str##mac
#define CT_HELPER_HELPER_STR(str, mac) CT_HELPER_HELPER_HELPER_STR(str, mac)
#define CT_HELPER_STR(str) CT_HELPER_HELPER_STR(str, __LINE__)

// ============ macros ============

#define CT_MEASURE_V(verbosity) ::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(__FUNCSIG__, "")

#define CT_MEASURE_VC(verbosity, category) ::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(__FUNCSIG__, category)

#define CT_MEASURE_VNC(verbosity, name, category) ::ChrTrcProfiler::ScopedTimer<verbosity> CT_HELPER_STR(STIMER_)(name " (" __FUNCTION__ ")", category)

#define CT_MEASURE_VN(verbosity, name) CT_MEASURE_VNC(verbosity, name, "")

#define CT_MEASURE() CT_MEASURE_V(0)

#define CT_MEASURE_C(category) CT_MEASURE_VC(0, category)

#define CT_MEASURE_NC(name, category) CT_MEASURE_VNC(0, name, category)

#define CT_MEASURE_N(name) CT_MEASURE_VN(0, name)
