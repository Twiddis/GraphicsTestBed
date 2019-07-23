// byte_Width: 16

#define VIEW_VECTOR(x) \
cbuffer ViewVector : register(b##x##) \
{ \
  float4 camera_pos; \
}
