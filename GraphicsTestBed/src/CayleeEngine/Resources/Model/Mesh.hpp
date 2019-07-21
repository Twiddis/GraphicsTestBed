#pragma once
#include "Resources/ResourceBase.hpp"

#include "Assimp/scene.h"
#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
class Mesh : public ResourceBase<Mesh>
{
public:
  Mesh();
  ~Mesh();
  
  void Build();
  void Bind();

  enum VertexAttributeType
  {
    Position,
    Normal,
    UV,
    COUNT
  };

    // Does nothing if data is null
  template <typename T>
  void CreateVertexBuffer(VertexAttributeType type,  const T *data, size_t num_elements);

private:
  static ID3D11Buffer *NULL_BUFFER;

  ID3D11Buffer *mPositionBuffer;
  ID3D11Buffer *mNormalBuffer;
  ID3D11Buffer *mUVBuffer;

  bool mIsBuilt;
};

template<typename T>
inline void Mesh::CreateVertexBuffer(VertexAttributeType type, const T *data, size_t num_elements)
{
  if (!data)
    return;

  ID3D11Buffer **buffer = &NULL_BUFFER;

  switch (type)
  {
  case Position:
    buffer = &mPositionBuffer;
    break;
  case Normal:
    buffer = &mNormalBuffer;
    break;
  case UV:
    buffer = &mUVBuffer;
    break;
  default:
    break;
  }

  if (buffer == &NULL_BUFFER) {
    err::AssertWarn(false, "Warning! Attempted to create a vertex buffer with an invalid attribute type!");
    return;
  }

  if (*buffer) {
    err::AssertWarn(false, "Warning! Attempted to create a vertex buffer that was already created!");
    return;
  }

  D3D11_BUFFER_DESC desc;
  {
    desc.ByteWidth = num_elements * sizeof(T);
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = sizeof(T);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  }

  D3D11_SUBRESOURCE_DATA res_data;
  {
    res_data.pSysMem = data;
    res_data.SysMemPitch = 0;
    res_data.SysMemSlicePitch = 0;
  }

  HRESULT hr = D3D::GetInstance()->mDevice->CreateBuffer(&desc, &res_data, buffer);
  err::HRWarn(hr, "Warning! Unable to create a vertex buffer");

}

}