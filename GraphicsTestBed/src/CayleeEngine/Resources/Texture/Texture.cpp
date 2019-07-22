#include "precompiled.hpp"
#include "Texture.hpp"

#include "Singletons/D3D/D3D.hpp"
#include "DirectXTK/DDSTextureLoader.h"

namespace CayleeEngine::res
{
  Texture::Texture(const wchar_t *filepath) : mShaderResourceView(nullptr)
  {
    ID3D11Device *dev = D3D::GetInstance()->mDevice;
    HRESULT hr = DirectX::CreateDDSTextureFromFile(dev, filepath, nullptr, &mShaderResourceView);
    err::AssertWarn(hr, "WARNING: Unable to load texture %S", filepath);
  }

  Texture::~Texture() 
  {
    SafeRelease(mShaderResourceView);
  }

  void Texture::Bind(UINT slot, Shader::Type shader_stage)
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;

    switch (shader_stage)
    {
    case Shader::Vertex:
      devcon->VSSetShaderResources(slot, 1, &mShaderResourceView);
      break;
    case Shader::Pixel:
      devcon->PSSetShaderResources(slot, 1, &mShaderResourceView);
      break;
    case Shader::Compute:
      devcon->CSSetShaderResources(slot, 1, &mShaderResourceView);
      break;
    default:
      break;
    };

  }
}