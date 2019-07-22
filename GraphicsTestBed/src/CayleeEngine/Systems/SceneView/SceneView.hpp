#pragma once
#include "Systems/System.hpp"

namespace CayleeEngine::sys
{
class SceneView : public System
{
public:
  SceneView();
  ~SceneView();

  void StartFrame();
  void Update(float dt);
  void EndFrame();

private:
  void Input();

  DirectX::Mouse::ButtonStateTracker mouse_tracker;
  DirectX::Keyboard::KeyboardStateTracker kb_tracker;
};

}