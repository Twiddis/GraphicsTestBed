// byte_Width: 16

// Render Setting:
// 0 - Render Normally
// 1 - Render 1st Buffer
// 2 - Render 2nd Buffer
// etc...
#define GBUFFER_RENDER_FLAGS(x) \
cbuffer GBufferRenderFlags : register(b##x##) \
{ \
  int render_setting; \
  int3 _padding; \
}
