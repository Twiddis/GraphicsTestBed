#include "precompiled.hpp"
#include "Window.hpp"

// Statically Defined Constants
static const LPCSTR gWindowName = "CayleeEngine";

namespace CayleeEngine
{
Window::Window()
{
    // Window Parameters
  {
    WNDCLASSEX wc;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = gWindowName;
    wc.cbSize = sizeof(WNDCLASSEX);

    err::HRFail(RegisterClassEx(&wc), "ERROR: Window Class Fail");
  }

  mScreenWidth = mResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
  mScreenHeight = mResolutionHeight = GetSystemMetrics(SM_CYSCREEN);

  // Create Window
  int posX = (GetSystemMetrics(SM_CXSCREEN) - mScreenWidth) / 2;
  int posY = (GetSystemMetrics(SM_CYSCREEN) - mScreenHeight) / 2;

  mWindowHandle = CreateWindowA(gWindowName, gWindowName,
    WS_OVERLAPPEDWINDOW | WS_VISIBLE, posX, posY, mScreenWidth, mScreenHeight,
    NULL, NULL, GetModuleHandle(NULL), NULL);

  err::AssertFail(mWindowHandle, "ERROR: Window Creation failed");
}

Window::~Window() 
{

}
LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
  switch (msg)
  {
  case WM_DESTROY:
    std::abort();
  default:
    break;
  }

  return DefWindowProc(hwnd, msg, WParam, LParam);
}
}