#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class Light : public ResourceBase<Light>
{
public:
  Light();
  ~Light();

  Vec4 mColor;
  Vec3 mPosition;
  float mRadius;
};



}