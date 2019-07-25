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
                                            mViewport()
{
  mViewport.Width = static_cast<float>(res_x);
  mViewport.Height = static_cast<float>(res_y);

  mViewport.MaxDepth = 1.0f;
  mViewport.MinDepth = 0.0f;
  mViewport.TopLeftX = 0.0f;
  mViewport.TopLeftY = 1.0f;

  mShadowMap = RenderTarget::Create(1, res_x, res_y);
}

Light::~Light()
{
}

void CayleeEngine::res::Light::ClearRenderTarget()
{
  mShadowMap->ClearRenderTarget();
}

void Light::BindForRender()
{
  mShadowMap->BindAsRenderTarget();
  D3D::GetInstance()->mDeviceContext->RSSetViewports(1, &mViewport);
  
}

void Light::BindForResource(UINT slot, bool is_compute)
{
  mShadowMap->BindAsResourceView(slot, is_compute);
}

void Light::GenerateViewMatrix()
{
  mViewMatrix = Mat::CreateLookAt(mPosition, mLookAtPosition, Vec3(0.0f, 1.0f, 0.0f));
}

void Light::GenerateProjectionMatrix()
{
  mProjectionMatrix = Mat::CreatePerspectiveFieldOfView(3.14f / 1.5f, 1.0f, 0.1f, 100.0f);
}


}