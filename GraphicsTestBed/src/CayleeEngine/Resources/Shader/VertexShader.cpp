#include "precompiled.hpp"
#include "VertexShader.hpp"

#include "Singletons/D3D/D3D.hpp"

// D3D11_INPUT_ELEMENT_DESC {
//   LPCSTR                     SemanticName;
//   UINT                       SemanticIndex;
//   DXGI_FORMAT                Format;
//   UINT                       InputSlot;
//   UINT                       AlignedByteOffset;
//   D3D11_INPUT_CLASSIFICATION InputSlotClass;
//   UINT                       InstanceDataStepRate;
// }
//////////////////////////////////////////////////////////////
static constexpr D3D11_INPUT_ELEMENT_DESC gInputLayoutElements[CayleeEngine::res::Mesh::COUNT] =
{
  // Position
{
  "POSITION",
  0,
  DXGI_FORMAT_R32G32B32_FLOAT,
  0,
  0,
  D3D11_INPUT_PER_VERTEX_DATA,
  0
},
// Normal
{
  "NORMAL",
  0,
  DXGI_FORMAT_R32G32B32_FLOAT,
  0,
  D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA,
  0
},
// UV
{
  "TEXCOORD",
  0,
  DXGI_FORMAT_R32G32B32_FLOAT,
  0,
  D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA,
  0
}
};

static const std::string INPUT_LAYOUT_PATH = "../src/CayleeEngine/Shaders/InputLayouts/";

namespace CayleeEngine::res
{

VertexShader::VertexShader() : mInputLayout(nullptr), mVertexShader(nullptr)
{
}

VertexShader::~VertexShader()
{
  SafeRelease(mInputLayout);
  SafeRelease(mVertexShader);
}

void VertexShader::LoadShader(const std::wstring &filepath)
{
    // Compile vertex shader
    /////////////////////////////////////////////////////
  ID3DBlob *shader_blob = nullptr;

  if (SUCCEEDED(CompileShader(filepath, "vs_5_0", &shader_blob))) {
    HRESULT hr = D3D::GetInstance()->mDevice->CreateVertexShader(shader_blob->GetBufferPointer(),
      shader_blob->GetBufferSize(), NULL,
      &mVertexShader);

    if (FAILED(hr)) {
      err::HRWarn(hr, "WARNING: Could not compile shader %S", filepath.c_str());
      SafeRelease(shader_blob);
    }
  }
  else {
    err::AssertWarn(false, "WARNING:! Could not compile shader %S", filepath.c_str());
    return;
  }

    // Search for input layout to create it
    //////////////////////////////////////////////////////
  std::ifstream in_file(filepath);
  std::stringstream str_stream;

  str_stream << in_file.rdbuf();
  std::string file_buffer = str_stream.str();

  const std::string search_input_layout = "#include \"../InputLayouts/";
  size_t search_pos = file_buffer.find(search_input_layout);
  if (search_pos != std::string::npos)
    search_pos += search_input_layout.length();
  
  size_t search_end = file_buffer.find('\"', search_pos);

  std::string input_layout_filename;
  
  if (search_pos == std::string::npos) {
    input_layout_filename = "default.hlsl";
  }
  else {
    input_layout_filename = file_buffer.substr(search_pos, search_end - search_pos);
  }


  CreateInputLayout(input_layout_filename, shader_blob);
  SafeRelease(shader_blob);

  if (mInputLayout && mVertexShader)
    mIsBuilt = true;
}

void VertexShader::Bind()
{
  if (!mIsBuilt) {
    err::AssertWarn(false, "WARNING: Attempted to bind unbuilt vertex shader");
    return;
  }

  D3D::GetInstance()->mDeviceContext->IASetInputLayout(mInputLayout);
  D3D::GetInstance()->mDeviceContext->VSSetShader(mVertexShader, NULL, 0);

  for (auto &it : mAssignedBuffers) {
    D3D::GetInstance()->mDeviceContext->VSSetConstantBuffers(it.first, 1, it.second->GetBuffer());
  }
}

ResourceID VertexShader::Create()
{
  ResourceID resource_id;
  resource_id.GenerateNewID();

  sResources.emplace(resource_id, std::make_unique<VertexShader>());
  return resource_id;
}

void VertexShader::CreateInputLayout(const std::string &filename, ID3DBlob *vertex_shader)
{
    // Parse input layout definition
    //////////////////////////////////////////////////////
  std::ifstream in_file(INPUT_LAYOUT_PATH + filename);
  std::stringstream str_stream;

  err::AssertWarn(in_file.is_open(), "Warning! Unable to open input layout for reading");
  if (!in_file.is_open())
    return;

  str_stream << in_file.rdbuf();
  std::string file_buffer = str_stream.str();

  const std::string search = "input_layout_flags: ";
  size_t search_pos = file_buffer.find(search) + search.length();
  size_t search_end = file_buffer.find('\n', search_pos);
  std::string flags = file_buffer.substr(search_pos, search_end - search_pos);
  
  std::vector<D3D11_INPUT_ELEMENT_DESC> input_elements;
  
  for (int i = 0; i < Mesh::VertexAttributeType::COUNT; ++i) 
  {
    char f = flags[i];
    if (std::stoi(&f))
      input_elements.push_back(gInputLayoutElements[i]);
  }

  
  HRESULT hr = D3D::GetInstance()->mDevice->CreateInputLayout(input_elements.data(),
                                                              input_elements.size(),
                                                              vertex_shader->GetBufferPointer(),
                                                              vertex_shader->GetBufferSize(),
                                                              &mInputLayout);

  err::HRWarn(hr, "Warning! Unable to create Input Layout");
}
}