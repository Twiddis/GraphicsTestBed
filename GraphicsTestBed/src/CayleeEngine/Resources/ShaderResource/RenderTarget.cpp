#include "precompiled.hpp"
#include "RenderTarget.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{
  static ID3D11ShaderResourceView *NULL_SRV[128] = { nullptr };
  static ID3D11RenderTargetView *NULL_RTV[8] = { nullptr };
  static ID3D11UnorderedAccessView *NULL_UAV[8] = { nullptr };

  const CD3D11_DEPTH_STENCIL_DESC DEFAULT_DEPTH_STENCIL_DESC{
    TRUE,                       // Depth: Enable
    D3D11_DEPTH_WRITE_MASK_ALL, // Depth: Write mask
    D3D11_COMPARISON_LESS,      // Depth: Comparison function
    FALSE,                      // Stencil: Enable
    0xFF,                       // Stencil: Read mask
    0xFF,                       // Stencil: Write mask
    D3D11_STENCIL_OP_KEEP,      // Stencil: Front fail operation
    D3D11_STENCIL_OP_INCR,      // Stencil: Front depth fail operation
    D3D11_STENCIL_OP_KEEP,      // Stencil: Front pass operation
    D3D11_COMPARISON_ALWAYS,    // Stencil: Front function
    D3D11_STENCIL_OP_KEEP,      // Stencil: Back fail operation
    D3D11_STENCIL_OP_DECR,      // Stencil: Back depth fail operation
    D3D11_STENCIL_OP_KEEP,      // Stencil: Back pass operation
    D3D11_COMPARISON_ALWAYS     // Stencil: Back function
  };

  RenderTarget::RenderTarget(size_t num_textures, int res_x, int res_y) :
                                   mResX(res_x), mResY(res_y),
                                   mBindStage(UNBOUND),
                                   mStartingSlot(0),
                                   mNumTextures(num_textures),
                                   mDepthStencil(nullptr),
                                   mDepthStencilView(nullptr),
                                   mDepthStencilState(nullptr),
                                   mBlendState(nullptr),
                                   mTextures(num_textures, nullptr),
                                   mShaderResourceViews(num_textures, nullptr),
                                   mRenderTargetViews(num_textures, nullptr),
                                   mUnorderedAccessViews(num_textures, nullptr)
  {
    BuildTextures(res_x, res_y);
  }

  RenderTarget::~RenderTarget()
  {
    ClearResources();
  }

  void RenderTarget::Unbind()
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;
    
    if (mBindStage == BindStage::UNBOUND)
      return;

    devcon->PSSetShaderResources(mStartingSlot, mNumTextures, NULL_SRV);
    devcon->OMSetRenderTargets(mNumTextures, NULL_RTV, mDepthStencilView);
    devcon->CSSetUnorderedAccessViews(mStartingSlot, mNumTextures, NULL_UAV, nullptr);
    
    mBindStage = BindStage::UNBOUND;
  }

  void RenderTarget::ClearRenderTarget()
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;
    const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    for (auto &rtv : mRenderTargetViews)
      devcon->ClearRenderTargetView(rtv, clear_color);

    devcon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  }

  void RenderTarget::Resize(int res_x, int res_y)
  {
    ClearResources();
    BuildTextures(res_x, res_y);
  }

  void RenderTarget::BindAsRenderTarget()
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;

    if (mBindStage == BindStage::RESOURCE || mBindStage == BindStage::UAV)
      Unbind();

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(mResX);
    vp.Height = static_cast<float>(mResY);

    vp.MaxDepth = 1.0f;
    vp.MinDepth = 0.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 1.0f;

    devcon->RSSetViewports(1, &vp);
    devcon->OMSetDepthStencilState(mDepthStencilState, 0xFF);
    devcon->OMSetRenderTargets(mNumTextures, mRenderTargetViews.data(), mDepthStencilView);
    devcon->OMSetBlendState(mBlendState, nullptr, 0xFFFFFFFF);

    mBindStage = BindStage::TARGET;
  }

  void RenderTarget::BindAsUnorderedAccessView(size_t starting_slot)
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;

    if (mBindStage == BindStage::RESOURCE || mBindStage == BindStage::TARGET)
      Unbind();

    mStartingSlot = starting_slot;

    devcon->CSSetUnorderedAccessViews(0, mNumTextures, mUnorderedAccessViews.data(), nullptr);
    mBindStage = BindStage::UAV;
  }

  void RenderTarget::BindAsResourceView(size_t starting_slot, bool is_compute)
  {
    ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;

    if (mBindStage == BindStage::TARGET || mBindStage == BindStage::UAV)
      Unbind();

    mStartingSlot = starting_slot;

    if (!is_compute)
      devcon->PSSetShaderResources(starting_slot, mNumTextures, mShaderResourceViews.data());
    else
      devcon->CSSetShaderResources(starting_slot, mNumTextures, mShaderResourceViews.data());

    mBindStage = BindStage::RESOURCE;
  }

  void RenderTarget::ClearResources()
  {
    SafeRelease(mDepthStencil);
    SafeRelease(mDepthStencilView);
    SafeRelease(mDepthStencilState);
    SafeRelease(mBlendState);

    for (auto &tex : mTextures)
      SafeRelease(tex);

    for (auto &view : mShaderResourceViews)
      SafeRelease(view);

    for (auto &view : mRenderTargetViews)
      SafeRelease(view);

    for (auto &view : mUnorderedAccessViews)
      SafeRelease(view);
  }

  void RenderTarget::BuildTextures(int res_x, int res_y)
  {
    ID3D11Device *dev = D3D::GetInstance()->mDevice;

    // Create Depth Buffer
    D3D11_TEXTURE2D_DESC depth_tex_desc;
    {
      depth_tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      depth_tex_desc.MiscFlags = 0;
      depth_tex_desc.Width = res_x;
      depth_tex_desc.Height = res_y;
      depth_tex_desc.ArraySize = 1;
      depth_tex_desc.MipLevels = 1;
      depth_tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      depth_tex_desc.SampleDesc.Count = 1;
      depth_tex_desc.SampleDesc.Quality = 0;
      depth_tex_desc.Usage = D3D11_USAGE_DEFAULT;
      depth_tex_desc.CPUAccessFlags = 0;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_view_desc;
    {
      depth_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      depth_view_desc.Texture2D.MipSlice = 0;
      depth_view_desc.Flags = 0;
      depth_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    }

    D3D11_BLEND_DESC blend_desc;
    {
      blend_desc.AlphaToCoverageEnable = false;
      blend_desc.IndependentBlendEnable = false;

      blend_desc.RenderTarget[0].BlendEnable = false;
      blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
      blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
      blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
      blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
      blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
      blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
    }


    err::HRWarn(dev->CreateTexture2D(&depth_tex_desc, nullptr, &mDepthStencil), "WARNING RTV STUFF");
    err::HRWarn(dev->CreateDepthStencilView(mDepthStencil, &depth_view_desc, &mDepthStencilView), "WARNING RTV STUFF");
    err::HRWarn(dev->CreateDepthStencilState(&DEFAULT_DEPTH_STENCIL_DESC, &mDepthStencilState), "warning RTV STUFFS");
    err::HRWarn(dev->CreateBlendState(&blend_desc, &mBlendState), "WARNING Blendstate RT messed up");

    SafeRelease(mDepthStencil);

    D3D11_TEXTURE2D_DESC desc;
    {
      desc.ArraySize  = mNumTextures;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
      desc.CPUAccessFlags = 0;
      desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; 
      desc.Height = res_y;
      desc.Width = res_x;
      desc.MipLevels = 1;
      desc.MiscFlags = 0;
      desc.SampleDesc.Count = 1;
      desc.SampleDesc.Quality = 0;
      desc.Usage = D3D11_USAGE_DEFAULT;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    {
      srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      srv_desc.Texture2D.MipLevels = 1;
      srv_desc.Texture2D.MostDetailedMip = 0;
    }

    D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
    {
      rtv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      rtv_desc.Texture2D.MipSlice = 0;
    }

    D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
    {
      uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
      uav_desc.Texture2D.MipSlice = 0;
    }

    for (size_t i = 0; i < mNumTextures; ++i) {
      err::HRWarn(dev->CreateTexture2D(&desc, nullptr, &mTextures[i]), "RenderTarget Failure");
      err::HRWarn(dev->CreateShaderResourceView(mTextures[i], &srv_desc, &mShaderResourceViews[i]), "RenderTarget Failure");
      err::HRWarn(dev->CreateRenderTargetView(mTextures[i], &rtv_desc, &mRenderTargetViews[i]), "RenderTarget Failure");
      err::HRWarn(dev->CreateUnorderedAccessView(mTextures[i], &uav_desc, &mUnorderedAccessViews[i]), "UAV OFFLINE");
    }

    for (auto &tex : mTextures)
      SafeRelease(tex);
  }
}