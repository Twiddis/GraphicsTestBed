#pragma once
#include "Systems/System.hpp"

#include "Resources/Model/Model.hpp"

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
  std::vector<res::Model::Key> mModels;
  
};

}