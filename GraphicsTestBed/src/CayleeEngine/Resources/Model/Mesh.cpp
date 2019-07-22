#include "precompiled.hpp"
#include "Mesh.hpp"

namespace CayleeEngine::res
{

Mesh::Mesh() : mIndexBuffer(nullptr), mNumIndices(0)
{
  for (size_t i = 0; i < VertexAttributeType::COUNT; ++i)
    mBufferArray[i] = nullptr;
 
}

Mesh::~Mesh()
{
  SafeRelease(mIndexBuffer);

  for (size_t i = 0; i < VertexAttributeType::COUNT; ++i)
    SafeRelease(mBufferArray[i]);

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
  size_t slot = 0;

  for (size_t i = 0; i < VertexAttributeType::COUNT; ++i) 
  {
    UINT offset = 0;
    if (mBufferArray[i])
      D3D::GetInstance()->mDeviceContext->IASetVertexBuffers(slot++, 1, &mBufferArray[i], &ATTRIBUTE_STRIDES[i], &offset);
  }

  D3D::GetInstance()->mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::CreateIndexBuffer(aiFace *faces, size_t num_faces)
{
  std::vector<unsigned int> indices;

  for (size_t i = 0; i < num_faces; ++i)
  {
    indices.push_back(faces[i].mIndices[0]);
    indices.push_back(faces[i].mIndices[1]);
    indices.push_back(faces[i].mIndices[2]);
  }

  D3D11_BUFFER_DESC desc;
  {
    desc.ByteWidth = sizeof(unsigned int) * indices.size();
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = sizeof(unsigned int);
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  }

  D3D11_SUBRESOURCE_DATA res_data;
  {
    res_data.pSysMem = indices.data();
    res_data.SysMemPitch = 0;
    res_data.SysMemSlicePitch = 0;
  }

  HRESULT hr = D3D::GetInstance()->mDevice->CreateBuffer(&desc, &res_data, &mIndexBuffer);
  err::HRWarn(hr, "Warning! Unable to create a index buffer");

  mNumIndices = indices.size();
}

}
