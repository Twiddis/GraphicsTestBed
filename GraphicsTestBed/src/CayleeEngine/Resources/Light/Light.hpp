#pragma once
#include "Resources/ResourceBase.hpp"

#include "Resources/ShaderResource/RenderTarget.hpp"

namespace CayleeEngine::res
{
class Light : public ResourceBase<Light>
{
public:
  Light(size_t res_x = 1024, size_t res_y = 1024);
  ~Light();

  Vec4 mColor;
  Vec3 mPosition;

  Vec3 mLookAtPosition;

  void Resize() {}

  void ClearRenderTarget();

  void BindForRender();
  void BindForResource(UINT slot);

  void GenerateViewMatrix();
  void GenerateProjectionMatrix();

  constexpr Mat& GetViewMatrix() { return mViewMatrix; };
  constexpr Mat& GetProjectionMatrix() { return mProjectionMatrix; }
  
private:

  Mat mViewMatrix;
  Mat mProjectionMatrix;

  res::RenderTarget::Key mShadowMap;

;
  D3D11_VIEWPORT mViewport;

};



}