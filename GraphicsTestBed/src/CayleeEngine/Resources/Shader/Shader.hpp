#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{

class Shader : public ResourceBase<Shader>
{
public:
  Shader();
  virtual ~Shader();

  enum Type
  {
    Vertex,
    Pixel,
    Compute,
    COUNT
  };
  
  virtual void LoadShader(const std::wstring &filepath) = 0;
  virtual void Bind() = 0;

  bool IsBuilt() { return mIsBuilt; }

protected:
  static HRESULT CompileShader(const std::wstring &filepath, const char *profile, ID3DBlob **blob);

  bool mIsBuilt;
};

}