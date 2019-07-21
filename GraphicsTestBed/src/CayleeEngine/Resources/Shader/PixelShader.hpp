#pragma once
#include "Resources/Shader/Shader.hpp"

namespace CayleeEngine::res
{

class PixelShader : public Shader
{
public:
  PixelShader();
  ~PixelShader();

  void LoadShader(const std::wstring &filepath) override;
  void Bind() override;

  static ResourceID Create();

private:
  ID3D11PixelShader *mPixelShader;

};

}