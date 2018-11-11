#include <comdef.h>
#include <iostream>
#include <Windows.h>

namespace err
{
#pragma warning (suppress : 4505)
static bool HRCheck(HRESULT hr)
{
  if (SUCCEEDED(hr))
    return true;

  _com_error error(hr);
  LPCTSTR msg = error.ErrorMessage();
  
  std::cerr << "HResult: " << msg << " ";

  return false;
}

template <typename ...Args>
void constexpr AssertFail(bool condition, const char *format, Args&& ...args)
{
  if (!condition) {
    fprintf(stderr, format, args...);
    assert(condition);
  }
}

template <typename ...Args>
void constexpr AssertWarn(bool condition, const char *format, Args&& ...args)
{
#ifdef DEBUG
  AssertFail(condition, format, args...);
#else
  if (!condition)
    fprintf(stderr, format, args...);
#endif
}

template <typename ...Args>
void constexpr HRFail(HRESULT hr, const char * format, Args&& ...args)
{
  if (!HRCheck(hr))
    AssertFail(false, format, args...);
}

template <typename ...Args>
void constexpr HRWarn(HRESULT hr, const char * format, Args&& ...args)
{
  if (!HRCheck(hr))
    AssertWarn(false, format, args...);
}
}
