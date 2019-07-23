#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class RenderTarget : public ResourceBase<RenderTarget>
{
public:
  RenderTarget(size_t num_textures, int res_x, int res_y);
  ~RenderTarget();

  void Unbind();

  void ClearRenderTarget();
  void Resize(int res_x, int res_y);

  void BindAsRenderTarget();
  void BindAsResourceView(size_t starting_slot);

  enum BindStage
  {
    UNBOUND,
    RESOURCE,
    TARGET,
    COUNT
  };

private:

  BindStage mBindStage;
  size_t mStartingSlot;

  void ClearResources();
  void BuildTextures(int res_x, int res_y);

  size_t mNumTextures;

  ID3D11Texture2D *mDepthStencil;
  ID3D11DepthStencilView *mDepthStencilView;
  ID3D11DepthStencilState *mDepthStencilState;
  ID3D11BlendState *mBlendState;

  std::vector<ID3D11Texture2D*> mTextures;
  std::vector<ID3D11ShaderResourceView*> mShaderResourceViews;
  std::vector<ID3D11RenderTargetView*> mRenderTargetViews;
};

}