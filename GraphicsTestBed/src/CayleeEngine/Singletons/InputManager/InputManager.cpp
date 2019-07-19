#include "precompiled.hpp"
#include "InputManager.hpp"

namespace CayleeEngine
{
InputManager::InputManager() : mQuitFlag(false)
{
}

LRESULT InputManager::WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
  switch (msg)
  {
  case WM_DESTROY:
    std::abort();
    break;
  case WM_QUIT:
    GetInstance()->mQuitFlag = true;
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, msg, WParam, LParam);
}
}