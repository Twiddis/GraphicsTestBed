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
void ShaderPipe::Build()
{
}
void ShaderPipe::AttachShader(Shader::Type type, Shader::Key shader)
{
  if (shader.IsValid()) {
    err::AssertWarn(!mShaders[type].IsValid(), "Warning! Replaced an already attached shader with a new one!");
    mShaders[type] = shader;
  }
}

void ShaderPipe::Bind()
{
  for (auto &shader : mShaders)
    shader->Bind();
}
}