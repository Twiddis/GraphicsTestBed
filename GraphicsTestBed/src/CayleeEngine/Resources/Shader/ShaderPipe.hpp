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
  
  void Bind();

private:
  std::vector<Shader::Key> mShaders;
};

}