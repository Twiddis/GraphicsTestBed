#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{

class Shader : public ResourceBase<Shader>
{
public:
  Shader();
  ~Shader();
  
  virtual void LoadShader(const std::wstring &filepath) = 0;

  virtual void Bind() = 0;

protected:
  static HRESULT CompileShader(const std::wstring &filepath, const char *profile, ID3DBlob **blob);
  
  virtual void Build() = 0;

  bool mIsBuilt;
};

}