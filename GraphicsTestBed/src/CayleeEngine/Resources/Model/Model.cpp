#include "precompiled.hpp"
#include "Model.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
  Model::Model() {}
  Model::~Model() {}

  void Model::Draw()
  {
    for (auto &mesh : mMeshes) {
      mesh->Bind();

      D3D::GetInstance()->mDeviceContext->DrawIndexed(mesh->mNumIndices, 0, 0);
    }
  }
}