#include "precompiled.hpp"
#include "InputManager.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"

namespace CayleeEngine
{
InputManager::InputManager() : mQuitFlag(false)
{
}

LRESULT InputManager::WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, WParam, LParam))
    return true;

  switch (msg)
  {
  case WM_DESTROY:
    GetInstance()->mQuitFlag = true;
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