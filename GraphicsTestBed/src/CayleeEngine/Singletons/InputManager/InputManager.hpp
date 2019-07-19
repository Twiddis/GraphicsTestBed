#pragma once
#include "Singletons/Singleton.hpp"

namespace CayleeEngine
{

class InputManager : public Singleton<InputManager>
{
public:
  InputManager();
  ~InputManager() = default;

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam);
  
    // NOTE: Don't touch, lol
  bool mQuitFlag;
};


}