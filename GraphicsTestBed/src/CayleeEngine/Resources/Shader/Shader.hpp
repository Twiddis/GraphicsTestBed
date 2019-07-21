#pragma once
#include "Resources/ResourceBase.hpp"
#include "Resources/D3DBuffer/D3DBuffer.hpp"

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

  void SearchAndAssignBuffers(const std::wstring &filepath);
  void AssignBuffer(size_t slot, res::D3DBuffer::Key buffer);

  bool IsBuilt() { return mIsBuilt; }

protected:
  static HRESULT CompileShader(const std::wstring &filepath, const char *profile, ID3DBlob **blob);

  bool mIsBuilt;

  std::map<size_t, res::D3DBuffer::Key> mAssignedBuffers;
};

}