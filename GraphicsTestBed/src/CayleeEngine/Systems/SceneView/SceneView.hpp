#pragma once
#include "Systems/System.hpp"

#include "Resources/Model/Model.hpp"

namespace CayleeEngine::sys
{
class SceneView : public System
{
public:
  SceneView() = default;
  ~SceneView() = default;

  void Update(float dt);

private:
  std::vector<res::Model::Key> mModels;
  
};

}