#include "precompiled.hpp"
#include "Core.hpp"
#include "Singletons//InputManager/InputManager.hpp"

namespace CayleeEngine
{
void Core::Run()
{
  InputManager::Initialize();

  while (!InputManager::GetInstance()->mQuitFlag)
  {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    float dt = 0.016f;
    for (auto &sys : mSystems) {
      if (sys->IsEnabled())
        sys->StartFrame();
    }

    for (auto &sys : mSystems) {
      if (sys->IsEnabled())
        sys->Update(dt);
    }

    for (auto &sys : mSystems) {
      if (sys->IsEnabled())
        sys->EndFrame();
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