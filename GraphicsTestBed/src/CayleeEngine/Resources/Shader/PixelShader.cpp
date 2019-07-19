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

  if (CompileShader(filepath, "vs_5_0", &shader_blob)) {
    D3D::GetInstance()->mDevice->CreatePixelShader(shader_blob->GetBufferPointer(), 
                                                   shader_blob->GetBufferSize(), NULL,
                                                   &mPixelShader);

    mIsBuilt = true;
  }
}

void PixelShader::Bind()
{
  D3D::GetInstance()->mDeviceContext->PSSetShader(mPixelShader, NULL, 0);
}
}