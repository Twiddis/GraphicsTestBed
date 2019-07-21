#pragma once

namespace err
{
template <typename ...Args>
void constexpr AssertFail(bool condition, const char *format, Args&& ...args);

template <typename ...Args>
void constexpr AssertWarn(bool condition, const char *format, Args&& ...args);

template <typename ...Args>
void constexpr HRFail(HRESULT hr, const char * format, Args&& ...args);

template <typename ...Args>
void constexpr HRWarn(HRESULT hr, const char * format, Args&& ...args);

void PrintLastWindowsError();
}

#include "ErrorReporting.inl"