#pragma once
#include "Resources/Shader/Shader.hpp"

#include "Resources/Model/Mesh.hpp"

namespace CayleeEngine::res
{

class VertexShader : public Shader
{
public:
  VertexShader();
  ~VertexShader();

  void LoadShader(const std::wstring &filepath) override;
  void Bind() override;

  static ResourceID Create();

private:
  void CreateInputLayout(const std::string &filename, ID3DBlob *vertex_shader);

  ID3D11InputLayout *mInputLayout;
  ID3D11VertexShader *mVertexShader;  

};

}