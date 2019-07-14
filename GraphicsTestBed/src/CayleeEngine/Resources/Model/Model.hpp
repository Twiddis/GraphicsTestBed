#pragma once
#include "Resources/ResourceBase.hpp"
#include "Resources/Model/Mesh.hpp"

namespace CayleeEngine::res
{
class Model : public ResourceBase<Model>
{
public:
  Model();
  ~Model();

private:
  std::vector<Mesh::Key> mMeshes;
};



}