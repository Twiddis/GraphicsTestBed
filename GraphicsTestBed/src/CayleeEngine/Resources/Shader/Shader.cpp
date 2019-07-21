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

void Shader::SearchAndAssignBuffers(const std::wstring &filepath)
{
  std::ifstream in_file(filepath);
  std::stringstream str_stream;

  str_stream << in_file.rdbuf();
  std::string file_buffer = str_stream.str();

  size_t search_pos = 0;
  size_t search_end = 0;
  size_t buffer_id = 0;
  const std::string search = "#include \"../ConstantBuffers/";

  do
  {
    search_pos = file_buffer.find(search, search_end);
    if (search_pos == std::string::npos)
      break;

    search_pos += search.length();
    search_end = file_buffer.find('\"', search_pos);

    std::string buffer_name = file_buffer.substr(search_pos, search_end - search_pos);
    AssignBuffer(buffer_id++, res::D3DBuffer::FindBufferWIthName(buffer_name));
  }
  while (true);
}

void Shader::AssignBuffer(size_t slot, res::D3DBuffer::Key buffer)
{
  if (buffer.IsValid())
    mAssignedBuffers.insert_or_assign(slot, buffer);

  err::AssertWarn(buffer.IsValid(), "WARNING: Attempted to assign a buffer that is not valid");
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