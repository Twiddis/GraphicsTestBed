#include "precompiled.hpp"
#include "D3DBuffer.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
D3DBuffer::D3DBuffer(const std::string &name, size_t byte_width) : mName(name), mByteWidth(byte_width), mBuffer(nullptr)
{
  D3D11_BUFFER_DESC desc;
  {
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = byte_width;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.Usage = D3D11_USAGE_DYNAMIC;
  
    HRESULT hr = D3D::GetInstance()->mDevice->CreateBuffer(&desc, nullptr, &mBuffer);
    err::HRWarn(hr, "Warning! Unable to create constant buffer: %s", name.c_str());

    if (SUCCEEDED(hr))
      mIsBuilt = true;
  }  
}

D3DBuffer::~D3DBuffer()
{
  SafeRelease(mBuffer);
}

ID3D11Buffer** D3DBuffer::GetBuffer()
{
  return &mBuffer;
}

void D3DBuffer::MapData(void *data)
{
  D3D11_MAPPED_SUBRESOURCE mapped_buffer;
  HRESULT hr = D3D::GetInstance()->mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

  if (FAILED(hr)) {
    err::AssertWarn(false, "WARNING: Failed to map buffer %s", mName.c_str());
    return;
  }

  std::memcpy(mapped_buffer.pData, data, mByteWidth);
  D3D::GetInstance()->mDeviceContext->Unmap(mBuffer, 0);
}

D3DBuffer::Key D3DBuffer::FindBufferWIthName(const std::string & name)
{
  for (auto &it : sResources) {
    if (it.second->mName == name)
      return D3DBuffer::Key(it.second->mID);
  }
  
  return D3DBuffer::Key();
}


}