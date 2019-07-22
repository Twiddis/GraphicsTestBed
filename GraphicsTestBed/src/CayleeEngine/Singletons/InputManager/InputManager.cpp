#include "precompiled.hpp"
#include "InputManager.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"

#include "Singletons/Window/Window.hpp"

namespace CayleeEngine
{
InputManager::InputManager() : mQuitFlag(false)
{
  mKeyboard = std::make_unique<DirectX::Keyboard>();
  mMouse = std::make_unique<DirectX::Mouse>();

  DirectX::Mouse::Get().SetWindow(Window::GetInstance()->GetWindowHandle());
  
}

LRESULT InputManager::WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, WParam, LParam))
    return true;

  switch (msg)
  {
  case WM_ACTIVATEAPP:
    DirectX::Keyboard::ProcessMessage(msg, WParam, LParam);
    DirectX::Mouse::ProcessMessage(msg, WParam, LParam);
    break;
  case WM_DESTROY:
    GetInstance()->mQuitFlag = true;
    break;
  case WM_QUIT:
    GetInstance()->mQuitFlag = true;
    break;

  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYUP:
    DirectX::Keyboard::ProcessMessage(msg, WParam, LParam);
    break;
  case WM_INPUT:
  case WM_MOUSEMOVE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_MOUSEWHEEL:
  case WM_XBUTTONDOWN:
  case WM_XBUTTONUP:
  case WM_MOUSEHOVER:
    DirectX::Mouse::ProcessMessage(msg, WParam, LParam);
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, msg, WParam, LParam);
}
}