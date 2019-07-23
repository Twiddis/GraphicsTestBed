#include "precompiled.hpp"
#include "Light.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::res
{

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

Light::Light(size_t res_x, size_t res_y) : mColor(1.0f, 1.0f, 1.0f, 1.0f),
                                            mPosition(0.0f, 0.0f, 0.0f),
                                            mLookAtPosition(0.0f, 0.0f, 0.0f),
                                            mViewMatrix(), mProjectionMatrix(),
                                            mDepthStencilView(nullptr),
                                            mRasterizerState(nullptr),
                                            mViewport()
{
  mViewport.Width = static_cast<float>(res_x);
  mViewport.Height = static_cast<float>(res_y);

  mViewport.MaxDepth = 1.0f;
  mViewport.MinDepth = 0.0f;
  mViewport.TopLeftX = 0.0f;
  mViewport.TopLeftY = 1.0f;

  CreateDepthBuffer(res_x, res_y);
}

Light::~Light()
{
  SafeRelease(mRasterizerState);

  SafeRelease(mDepthStencilView);

  SafeRelease(mShaderResourceView);
  SafeRelease(mRenderTargetView);
}

void Light::BindAsRenderTarget()
{
  ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;

  devcon->OMSetDepthStencilState(mDepthStencilState, 0xFF);
  devcon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
  
  //devcon->OMSetBlendState(mBlendState, nullptr, 0xFFFFFFFF);
}

void Light::BindAsResource()
{

}

void Light::GenerateViewMatrix()
{
  mViewMatrix = Mat::CreateLookAt(mPosition, mLookAtPosition, Vec3(0.0f, 1.0f, 0.0f));
}

void Light::GenerateProjectionMatrix()
{
  mProjectionMatrix = Mat::CreatePerspective(mViewport.Width, mViewport.Height, 0.1f, 1000.0f);
}

void Light::CreateDepthBuffer(size_t res_x, size_t res_y)
{
  SafeRelease(mDepthStencilView);
  SafeRelease(mShaderResourceView);

  ID3D11Device *dev = D3D::GetInstance()->mDevice; 
  ID3D11Texture2D *depth_buffer = nullptr;

  {
    D3D11_TEXTURE2D_DESC desc;

    desc.Width = res_x;
    desc.Height = res_y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 4;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    err::HRWarn(dev->CreateTexture2D(&desc, NULL, &depth_buffer),
      "WARNING: Unable To Create Depth Stencil Buffer");
  }

  {
    D3D11_DEPTH_STENCIL_VIEW_DESC desc;

    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    desc.Texture2D.MipSlice = 0;
    desc.Flags = 0;

    err::HRWarn(dev->CreateDepthStencilView(depth_buffer, &desc, &mDepthStencilView),
      "WARNING: Unable To Create Depth Stencil View");
  }

  err::HRWarn(dev->CreateDepthStencilState(&DEFAULT_DEPTH_STENCIL_DESC, &mDepthStencilState), "warning LIGHT STUFFS");
  ID3D11Texture2D *shadow_map_buffer = nullptr;

  {
    D3D11_TEXTURE2D_DESC desc;

    desc.Width = res_x;
    desc.Height = res_y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 4;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    err::HRWarn(dev->CreateTexture2D(&desc, NULL, &shadow_map_buffer),
      "WARNING: Unable To Create Depth Stencil Buffer");
  }

  {
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;

    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipLevels = 1;
    desc.Texture2D.MostDetailedMip = 0;

    err::HRWarn(dev->CreateShaderResourceView(depth_buffer, &desc, &mShaderResourceView),
        "WARNING: Unable to create shade rresource view");
  }

  {
    D3D11_RENDER_TARGET_VIEW_DESC desc;

    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;

    err::HRWarn(dev->CreateRenderTargetView(shadow_map_buffer, &desc, &mRenderTargetView), "RenderTarget Failure");
  }

  SafeRelease(depth_buffer);
  SafeRelease(shadow_map_buffer);
}


}