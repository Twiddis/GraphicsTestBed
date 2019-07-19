#pragma once
#include "Singletons/Singleton.hpp"

namespace CayleeEngine
{
class D3D : public Singleton<D3D>
{
public:
  D3D();
  ~D3D();

private:
  void InitializeDirectX();

  void CreateRenderTargetView();
  void CreateRasterizerState();
  void CreateDepthStencilState();
  void CreateDepthBuffer();
  void CreateDepthStencilView();
  void CreateBlendState();
  void SetViewport();

public:
  ID3D11Device *mDevice;
  ID3D11DeviceContext *mDeviceContext;

  IDXGISwapChain          *mSwapChain;
  ID3D11RenderTargetView  *mRenderTargetView;
  ID3D11Texture2D         *mDepthStencilBuffer;
  ID3D11DepthStencilState *mDepthStencilState;
  ID3D11DepthStencilView  *mDepthStencilView;
  ID3D11RasterizerState   *mRasterizerState;
  ID3D11BlendState        *mBlendState;

  D3D_FEATURE_LEVEL mFeatureLevel;
  D3D_DRIVER_TYPE mDriverType;
};
}