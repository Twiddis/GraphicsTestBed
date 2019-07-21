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

  void AppendMesh(Mesh::Key mesh) { mMeshes.push_back(mesh); };

private:
  std::vector<Mesh::Key> mMeshes;
};



}