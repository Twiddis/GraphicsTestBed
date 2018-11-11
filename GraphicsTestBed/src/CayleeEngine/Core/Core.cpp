#include "precompiled.hpp"
#include "Core.hpp"

namespace CayleeEngine
{

void Core::Run()
{
  float dt = 0.016f;
  for (auto &sys : mSystems) {
    if (sys->IsEnabled())
      sys->Update(dt);
  }
}

void Core::Shutdown()
{
  while (!mSystems.empty())
    mSystems.pop_back();
}
}