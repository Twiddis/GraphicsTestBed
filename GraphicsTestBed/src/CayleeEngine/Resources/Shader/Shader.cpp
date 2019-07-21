#include "precompiled.hpp"
#include "Shader.hpp"

#include <d3dcompiler.h>

namespace CayleeEngine::res
{

Shader::Shader() : mIsBuilt(false)
{
}
Shader::~Shader()
{
}

HRESULT Shader::CompileShader(const std::wstring &filepath, const char *profile, ID3DBlob **blob)
{
  *blob = nullptr;

  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  flags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* shader_blob = nullptr;
  ID3DBlob* error_blob = nullptr;
  HRESULT hr = D3DCompileFromFile(filepath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
    "main", profile, flags, 0, &shader_blob, &error_blob);

  if (FAILED(hr))
  {
    err::AssertWarn(false, "Unable to compile shader %S, %s", filepath.c_str(), error_blob->GetBufferPointer());
    err::PrintLastWindowsError();

    SafeRelease(error_blob);
    SafeRelease(shader_blob);

    return hr;
  }

  *blob = shader_blob;
  return hr;
}
}