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
  std::unique_ptr<ID3D11Device> mDevice;
  std::unique_ptr<ID3D11DeviceContext> mDeviceContext;

  std::unique_ptr<IDXGISwapChain> mSwapChain;
  std::unique_ptr<ID3D11RenderTargetView> mRenderTargetView;
  std::unique_ptr<ID3D11Texture2D> mDepthStencilBuffer;
  std::unique_ptr<ID3D11DepthStencilState> mDepthStencilState;
  std::unique_ptr<ID3D11DepthStencilView> mDepthStencilView;
  std::unique_ptr<ID3D11RasterizerState> mRasterizerState;
  std::unique_ptr<ID3D11BlendState> mBlendState;

  D3D_FEATURE_LEVEL mFeatureLevel;
  D3D_DRIVER_TYPE mDriverType;
};
}