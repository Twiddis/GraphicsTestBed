#include "precompiled.hpp"
#include "ComputeShader.hpp"

#include "Singletons/D3D/D3D.hpp"


namespace CayleeEngine::res
{

ComputeShader::ComputeShader() : mComputeShader(nullptr)
{
}

ComputeShader::~ComputeShader()
{
  SafeRelease(mComputeShader);
}

void ComputeShader::LoadShader(const std::wstring &filepath)
{
    // Compile vertex shader
    /////////////////////////////////////////////////////
  ID3DBlob *shader_blob = nullptr;

  if (SUCCEEDED(CompileShader(filepath, "cs_5_0", &shader_blob))) {
    HRESULT hr = D3D::GetInstance()->mDevice->CreateComputeShader(shader_blob->GetBufferPointer(),
      shader_blob->GetBufferSize(), NULL,
      &mComputeShader);

    if (FAILED(hr)) {
      err::HRWarn(hr, "WARNING: Could not compile shader %S", filepath.c_str());
      SafeRelease(shader_blob);
    }
  }
  else {
    err::AssertWarn(false, "WARNING:! Could not compile shader %S", filepath.c_str());
    return;
  }

  SafeRelease(shader_blob);

  if (mComputeShader)
    mIsBuilt = true;
}

void ComputeShader::Bind()
{
  if (!mIsBuilt) {
    err::AssertWarn(false, "WARNING: Attempted to bind unbuilt compute shader");
    return;
  }

  D3D::GetInstance()->mDeviceContext->CSSetShader(mComputeShader, NULL, 0);
  for (auto &it : mAssignedBuffers) {
    D3D::GetInstance()->mDeviceContext->CSSetConstantBuffers(it.first, 1, it.second->GetBuffer());
  }
}

ResourceID ComputeShader::Create()
{
  ResourceID resource_id;
  resource_id.GenerateNewID();

  sResources.emplace(resource_id, std::make_unique<ComputeShader>());
  return resource_id;
}


}