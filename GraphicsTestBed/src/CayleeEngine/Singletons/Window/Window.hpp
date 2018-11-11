#pragma once
#include "Singletons/Singleton.hpp"

namespace CayleeEngine
{
class Window : public Singleton<Window>
{
public:
  Window();
  ~Window();
  
  int GetScreenWidth()      const { return mScreenWidth; }
  int GetScreenHeight()     const { return mScreenHeight; }
  int GetResolutionWidth()  const { return mResolutionWidth; }
  int GetResolutionHeight() const { return mResolutionHeight; }

  HWND GetWindowHandle() const { return mWindowHandle; }

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam);

  HWND mWindowHandle;

  int mScreenWidth;
  int mScreenHeight;
  int mResolutionWidth;
  int mResolutionHeight;
};
}