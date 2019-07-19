#include "precompiled.hpp"
#include "Core.hpp"
#include "Singletons//InputManager/InputManager.hpp"

namespace CayleeEngine
{
void Core::Run()
{
  InputManager::Initialize();

  float dt = 0.016f;
  while (!InputManager::GetInstance()->mQuitFlag)
  {
    for (auto &sys : mSystems) {
      if (sys->IsEnabled())
        sys->Update(dt);
    }
  }
}

void Core::Shutdown()
{
  while (!mSystems.empty())
    mSystems.pop_back();

  InputManager::Shutdown();
}
}