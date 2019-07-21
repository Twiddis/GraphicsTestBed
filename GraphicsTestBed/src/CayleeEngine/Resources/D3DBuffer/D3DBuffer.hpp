#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class D3DBuffer : public ResourceBase<D3DBuffer>
{
public:
  D3DBuffer(const std::string &name, size_t byte_width);
  ~D3DBuffer();

  ID3D11Buffer** GetBuffer();
  void MapData(void *data);

  bool IsBuilt() { return mIsBuilt; }

  static D3DBuffer::Key FindBufferWIthName(const std::string &name);

public:
  std::string mName;

protected:
  bool mIsBuilt;

private:
  size_t mByteWidth;
  ID3D11Buffer *mBuffer;
};

}