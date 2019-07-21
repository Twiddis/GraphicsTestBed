#include "precompiled.hpp"
#include "Entity.hpp"

namespace CayleeEngine::res
{
  Entity::Entity() : mPosition(0.0f, 0.0f, 0.0f), 
                     mScale(1.0f, 1.0f, 1.0f), 
                     mRotation(0.0f, 0.0f, 0.0f)
  {
  }

  Entity::~Entity() {}
}