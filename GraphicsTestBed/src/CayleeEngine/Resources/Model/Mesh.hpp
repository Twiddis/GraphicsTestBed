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

  static constexpr UINT ATTRIBUTE_STRIDES[VertexAttributeType::COUNT]
    = {sizeof(float) * 3,
       sizeof(float) * 3,
       sizeof(float) * 3};

    // Does nothing if data is null
  template <typename T>
  void CreateVertexBuffer(VertexAttributeType type,  const T *data, size_t num_elements);
  void CreateIndexBuffer(aiFace *faces, size_t num_faces);

  int mNumIndices;
private:
  ID3D11Buffer *mIndexBuffer;
  ID3D11Buffer* mBufferArray[VertexAttributeType::COUNT];

  bool mIsBuilt;
};

template<typename T>
inline void Mesh::CreateVertexBuffer(VertexAttributeType type, const T *data, size_t num_elements)
{
  if (!data)
    return;

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

  HRESULT hr = D3D::GetInstance()->mDevice->CreateBuffer(&desc, &res_data, &mBufferArray[type]);
  err::HRWarn(hr, "Warning! Unable to create a vertex buffer");

}

}