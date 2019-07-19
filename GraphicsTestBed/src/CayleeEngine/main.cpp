#include "precompiled.hpp"
#include "Core/Core.hpp"
#include "Singletons/InputManager/InputManager.hpp"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
  AllocConsole();
  freopen("CONOUT$", "w", stderr);

  CayleeEngine::Core core;

  core.StartSystem<CayleeEngine::sys::Graphics>();

  core.Run();
  
  core.Shutdown();

  return 0;
}
