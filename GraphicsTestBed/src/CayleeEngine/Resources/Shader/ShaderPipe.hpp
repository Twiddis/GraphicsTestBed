#pragma once
#include "Resources/ResourceBase.hpp"

#include "Resources/Shader/Shader.hpp"


namespace CayleeEngine::res
{

class ShaderPipe : public ResourceBase<ShaderPipe>
{
public:
  ShaderPipe();
  ~ShaderPipe();
  
  void Build();
  
    // TODO:
  void SetConstantBuffers() {}
    // TODO:
  void SetRenderTargetView() {}

  void AttachShader(Shader::Type type, Shader::Key shader);
  bool HasShader(Shader::Type type) { return mShaders[type]; };

  void Bind();

private:
  Shader::Key mShaders[Shader::Type::COUNT];
};

}