#include "precompiled.hpp"
#include "D3D.hpp"

#include "Singletons/Window/Window.hpp"

namespace CayleeEngine
{
D3D::D3D() : mDevice(nullptr), mDeviceContext(nullptr),
             mSwapChain(nullptr), mDepthStencilBuffer(nullptr),
             mDepthStencilState(nullptr), mDepthStencilView(nullptr),
             mRasterizerState(nullptr), mBlendState(nullptr)
{
  InitializeDirectX();
  CreateRenderTargetView();
  CreateRasterizerState();
  CreateDepthStencilState();
  CreateDepthBuffer();
  CreateDepthStencilView();
  CreateBlendState();
  SetViewport();
}

D3D::~D3D()
{
  SafeRelease(mSwapChain);
  SafeRelease(mRenderTargetView);
  SafeRelease(mDepthStencilBuffer);
  SafeRelease(mDepthStencilState);
  SafeRelease(mDepthStencilView);
  SafeRelease(mRasterizerState);
  SafeRelease(mBlendState);
}

void D3D::BindRenderTarget()
{
  mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
  mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 0xFF);
  mDeviceContext->OMSetBlendState(mBlendState, nullptr, 0xFFFFFFFF);

  SetViewport();
}

void D3D::ClearRenderTarget()
{
  const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  mDeviceContext->ClearRenderTargetView(mRenderTargetView, clear_color);
  mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D::InitializeDirectX()
{
  // Specify Drivers and Features
  D3D_DRIVER_TYPE driver_types[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_REFERENCE
  };

  D3D_FEATURE_LEVEL feature_levels[] =
  {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_0
  };

  unsigned total_driver_types = ARRAYSIZE(driver_types);
  unsigned total_feature_levels = ARRAYSIZE(feature_levels);

  HRESULT hr = S_OK;
  DXGI_SWAP_CHAIN_DESC sd;
  {
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = 1;
    sd.BufferDesc.Width = Window::GetInstance()->GetResolutionWidth();
    sd.BufferDesc.Height = Window::GetInstance()->GetResolutionHeight();
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = Window::GetInstance()->GetWindowHandle();
    sd.Windowed = true;
    sd.SampleDesc.Count = 4;
    sd.SampleDesc.Quality = 0;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  }

  unsigned creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
  creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  for (unsigned i = 0; i < total_driver_types; ++i) {
    hr = D3D11CreateDeviceAndSwapChain(0, driver_types[i], 0, creation_flags, feature_levels,
      total_feature_levels, D3D11_SDK_VERSION, &sd, &mSwapChain,
      &mDevice, &mFeatureLevel, &mDeviceContext);

    if (SUCCEEDED(hr)) {
      mDriverType = driver_types[i];
      break;
    }
  }

  err::HRFail(hr, "ERROR: Failed to create the Direct3D Device!");

  
}

void D3D::CreateRenderTargetView()
{
  DXGI_SWAP_CHAIN_DESC sd;
  mSwapChain->GetDesc(&sd);

  ID3D11Texture2D *back_buffer_texture;
  D3D11_RENDER_TARGET_VIEW_DESC desc;

  err::HRFail(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_texture), 
              "ERROR: Failed to get the Swap Chain Back Buffer");

  desc.Format = sd.BufferDesc.Format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

  err::HRFail(mDevice->CreateRenderTargetView(back_buffer_texture, &desc, &mRenderTargetView),
              "ERROR: Failed to create the Render Target View");

  SafeRelease(back_buffer_texture);
}

void D3D::CreateRasterizerState()
{
  D3D11_RASTERIZER_DESC desc;

  desc.AntialiasedLineEnable = false;
  desc.CullMode = D3D11_CULL_BACK;
  desc.DepthBias = 0;
  desc.DepthBiasClamp = 0.0f;
  desc.DepthClipEnable = true;
  desc.FillMode = D3D11_FILL_SOLID;
  desc.FrontCounterClockwise = true; // RH Coordinate System
  desc.MultisampleEnable = false;
  desc.ScissorEnable = false;
  desc.SlopeScaledDepthBias = 0.0f;

  err::HRFail(mDevice->CreateRasterizerState(&desc, &mRasterizerState),
              "Unable to Create Rasterizer State");
  
  mDeviceContext->RSSetState(mRasterizerState);
}

void D3D::CreateDepthStencilState()
{
  D3D11_DEPTH_STENCIL_DESC desc;

  desc.DepthEnable = false;
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  desc.DepthFunc = D3D11_COMPARISON_LESS;

  desc.StencilEnable = true;
  desc.StencilReadMask = 0xFF;
  desc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing.
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing.
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  err::HRFail(mDevice->CreateDepthStencilState(&desc, &mDepthStencilState),
              "ERROR: Unable to Create Depth Stencil State");
 
  mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
}

void D3D::CreateDepthBuffer()
{
  D3D11_TEXTURE2D_DESC desc;

  desc.Width = Window::GetInstance()->GetResolutionWidth();
  desc.Height = Window::GetInstance()->GetResolutionHeight();
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  desc.SampleDesc.Count = 4;
  desc.SampleDesc.Quality = 0;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  err::HRFail(mDevice->CreateTexture2D(&desc, NULL, &mDepthStencilBuffer),
              "ERROR: Unable To Create Depth Stencil Buffer");
}

void D3D::CreateDepthStencilView()
{
  D3D11_DEPTH_STENCIL_VIEW_DESC desc;

  desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  desc.Texture2D.MipSlice = 0;
  desc.Flags = 0;

  err::HRFail(mDevice->CreateDepthStencilView(mDepthStencilBuffer, &desc, &mDepthStencilView),
              "ERROR: Unable To Create Depth Stencil View");
}

void D3D::CreateBlendState()
{
  D3D11_BLEND_DESC desc;

  desc.AlphaToCoverageEnable = false;
  desc.IndependentBlendEnable = false;

  for (unsigned i = 0; i < 8; ++i) {
    desc.RenderTarget[i].BlendEnable = true;
    desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
    desc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
    desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[i].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
  }

  err::HRFail(mDevice->CreateBlendState(&desc, &mBlendState),
              "ERROR: Unable to Create Blend State");

  mDeviceContext->OMSetBlendState(mBlendState, NULL, 0xFFFFFFFF);
}

void D3D::SetViewport()
{
  D3D11_VIEWPORT vp;
  vp.Width = static_cast<float>(Window::GetInstance()->GetResolutionWidth());
  vp.Height = static_cast<float>(Window::GetInstance()->GetResolutionHeight());
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0.0f;
  vp.TopLeftY = 1.0f;

  mDeviceContext->RSSetViewports(1, &vp);
}

}