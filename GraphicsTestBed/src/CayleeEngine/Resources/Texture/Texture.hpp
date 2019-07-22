#pragma once
#include "Resources/ResourceBase.hpp"
#include "Resources/Shader/Shader.hpp"

namespace CayleeEngine::res
{
class Texture : public ResourceBase<Texture>
{
public:
  Texture(const wchar_t *filepath);
  ~Texture();

  void Bind(UINT slot, Shader::Type shader_stage);
  static void ClearBinding(UINT slot, Shader::Type shader_stage);
private:
  ID3D11ShaderResourceView *mShaderResourceView;
};



}