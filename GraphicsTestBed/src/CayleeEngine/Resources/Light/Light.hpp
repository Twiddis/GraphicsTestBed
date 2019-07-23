#pragma once
#include "Resources/ResourceBase.hpp"

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

  void BindAsRenderTarget();
  void BindAsResource();

  void GenerateViewMatrix();
  void GenerateProjectionMatrix();

  constexpr Mat& GetViewMatrix() { return mViewMatrix; };
  constexpr Mat& GetProjectionMatrix() { return mProjectionMatrix; }

private:
  void CreateDepthBuffer(size_t res_x, size_t res_y);

private:
  Mat mViewMatrix;
  Mat mProjectionMatrix;

  ID3D11DepthStencilView *mDepthStencilView;
  ID3D11DepthStencilState *mDepthStencilState;

  ID3D11RenderTargetView *mRenderTargetView;
  ID3D11ShaderResourceView *mShaderResourceView;

  ID3D11RasterizerState *mRasterizerState;
  D3D11_VIEWPORT mViewport;

};



}