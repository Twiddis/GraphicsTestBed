#include "precompiled.hpp"
#include "Mesh.hpp"

namespace CayleeEngine::res
{
ID3D11Buffer* Mesh::NULL_BUFFER = NULL;

Mesh::Mesh() : mPositionBuffer(nullptr), mNormalBuffer(nullptr), mUVBuffer(nullptr), mIsBuilt(false)
{
}

Mesh::~Mesh()
{
  SafeRelease(mPositionBuffer);
  SafeRelease(mNormalBuffer);
  SafeRelease(mUVBuffer);
}

void Mesh::Build()
{
  if (mIsBuilt) {
    err::AssertWarn(mIsBuilt, "Warning: Attempted to build an already built mesh");
    return;
  }

  mIsBuilt = true;
}

void Mesh::Bind()
{
}

}
