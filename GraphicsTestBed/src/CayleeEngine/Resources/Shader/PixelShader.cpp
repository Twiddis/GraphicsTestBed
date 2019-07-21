#include "precompiled.hpp"
#include "PixelShader.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
PixelShader::PixelShader() : mPixelShader(nullptr)
{
}

PixelShader::~PixelShader()
{
  SafeRelease(mPixelShader);
}

void PixelShader::LoadShader(const std::wstring &filepath)
{
  ID3DBlob *shader_blob = nullptr;

  if (SUCCEEDED(CompileShader(filepath, "ps_5_0", &shader_blob))) {
    HRESULT hr = D3D::GetInstance()->mDevice->CreatePixelShader(shader_blob->GetBufferPointer(), 
                                                                shader_blob->GetBufferSize(), NULL,
                                                                &mPixelShader);

    err::HRWarn(hr, "Warning! Could not compile shader %S", filepath.c_str());

    if (SUCCEEDED(hr))
      mIsBuilt = true;
  }
}

void PixelShader::Bind()
{
  if (!mIsBuilt) {
    err::AssertWarn(false, "Warning! Attempted to bind unbuilt pixel shader");
    return;
  }

  D3D::GetInstance()->mDeviceContext->PSSetShader(mPixelShader, NULL, 0);
}

ResourceID PixelShader::Create()
{
  ResourceID resource_id;
  resource_id.GenerateNewID();

  sResources.emplace(resource_id, std::make_unique<PixelShader>());
  return resource_id;
}
}