#pragma once
#include "Resources/Shader/Shader.hpp"

namespace CayleeEngine::res
{

class VertexShader : public Shader
{
public:
  VertexShader();
  ~VertexShader();

  void LoadShader(const std::wstring &filepath) override;
  void Bind() override;

private:
  ID3D11VertexShader *mVertexShader;
};

}