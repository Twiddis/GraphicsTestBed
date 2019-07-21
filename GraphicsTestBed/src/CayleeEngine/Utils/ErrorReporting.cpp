#include "precompiled.hpp"
#include "ErrorReporting.hpp"

void err::PrintLastWindowsError()
{
  DWORD error = ::GetLastError();
  if (error == 0)
    return; //No error message has been recorded

  LPSTR message_buffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message_buffer, 0, NULL);

  std::string message(message_buffer, size);
  std::cerr << message << std::endl;

  //Free the buffer.
  LocalFree(message_buffer);
}
