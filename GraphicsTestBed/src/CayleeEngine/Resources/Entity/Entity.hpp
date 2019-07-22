#pragma once
#include "Resources/ResourceBase.hpp"

#include "Resources/Shader/ShaderPipe.hpp"
#include "Resources/Model/Model.hpp"
#include "Resources/Texture/Texture.hpp"

namespace CayleeEngine::res
{
class Entity : public ResourceBase<Entity>
{
public:
  Entity();
  ~Entity();

  Vec3 mPosition;
  Vec3 mScale;
  Vec3 mRotation;

  Mat& GetTransform();
  Mat mTransform;
  bool mIsTransformCalculated;

  res::Model::Key mModel;
  res::Texture::Key mTexture;
  res::ShaderPipe::Key mShaderProgram;
};

}