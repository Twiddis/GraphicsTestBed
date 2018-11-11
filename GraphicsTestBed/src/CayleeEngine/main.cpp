#include "precompiled.hpp"
#include "Core/Core.hpp"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
  AllocConsole();
  freopen("CONOUT$", "w", stderr);

  CayleeEngine::Core core;

  core.StartSystem<CayleeEngine::sys::Graphics>();
  core.StartSystem<CayleeEngine::sys::Input>();

  return 0;
}
