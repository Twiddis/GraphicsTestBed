#include "precompiled.hpp"
#include "Entity.hpp"

namespace CayleeEngine::res
{
  Entity::Entity() : mPosition(0.0f, 0.0f, 0.0f), 
                     mScale(1.0f, 1.0f, 1.0f), 
                     mRotation(0.0f, 0.0f, 0.0f),
                     mTransform(),
                     mIsTransformCalculated(false)
  {
  }

  Entity::~Entity() {}

  Mat & Entity::GetTransform()
  {
    if (!mIsTransformCalculated) {
      mTransform = Mat::CreateScale(mScale)  *
                   Mat::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z) *
                   Mat::CreateTranslation(mPosition);

    }    


    return mTransform;
  }
}