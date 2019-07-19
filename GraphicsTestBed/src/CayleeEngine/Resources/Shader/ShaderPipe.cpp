#include "precompiled.hpp"
#include "ShaderPipe.hpp"



namespace CayleeEngine::res
{
ShaderPipe::ShaderPipe()
{
}
ShaderPipe::~ShaderPipe()
{
}
void ShaderPipe::Bind()
{
  for (auto &shader : mShaders)
    shader->Bind();
}
}