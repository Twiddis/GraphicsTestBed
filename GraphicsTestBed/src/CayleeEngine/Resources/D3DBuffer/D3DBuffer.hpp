#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class D3DBuffer : public ResourceBase<D3DBuffer>
{
public:
  D3DBuffer();
  ~D3DBuffer();

protected:
  bool mIsBuilt;
};

}