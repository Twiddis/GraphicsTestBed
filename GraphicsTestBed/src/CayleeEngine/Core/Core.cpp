#include "precompiled.hpp"
#include "Core.hpp"

#include "Systems/Graphics/Graphics.hpp"
#include "Systems/Input/Input.hpp"

namespace CayleeEngine
{
Core::Core()
{
}

Core::~Core()
{
}

void Core::StartSystem(std::unique_ptr<System> &system)
{
  mSystems.push_back(std::move(system));
  mSystems.back()->Initialize();
  mSystems.back()->Enable();
}

void Core::Run()
{
  float dt = 0.016f;
  for (auto &sys : mSystems) {
    sys->Update(dt);
  }
}

void Core::Shutdown()
{
}
}