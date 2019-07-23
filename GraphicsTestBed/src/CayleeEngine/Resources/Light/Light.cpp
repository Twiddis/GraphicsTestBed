#include "precompiled.hpp"
#include "Light.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
  Light::Light() : mColor(1.0f, 1.0f, 1.0f, 1.0f), mPosition(0.0f, 0.0f, 0.0f), mRadius(0.0f)
  {
  }

  Light::~Light() {}


}