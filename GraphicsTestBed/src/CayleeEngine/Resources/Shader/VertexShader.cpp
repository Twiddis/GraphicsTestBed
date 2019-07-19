#include "precompiled.hpp"
#include "VertexShader.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{

VertexShader::VertexShader() : mVertexShader(nullptr)
{
}

VertexShader::~VertexShader()
{
  SafeRelease(mVertexShader);
}

void VertexShader::LoadShader(const std::wstring &filepath)
{
  ID3DBlob *shader_blob = nullptr;

  if (CompileShader(filepath, "vs_5_0", &shader_blob)) {
    D3D::GetInstance()->mDevice->CreateVertexShader(shader_blob->GetBufferPointer(), 
                                                    shader_blob->GetBufferSize(), NULL,
                                                    &mVertexShader);

    mIsBuilt = true;
  }
}

void VertexShader::Bind()
{
  D3D::GetInstance()->mDeviceContext->VSSetShader(mVertexShader, NULL, 0);
}

}