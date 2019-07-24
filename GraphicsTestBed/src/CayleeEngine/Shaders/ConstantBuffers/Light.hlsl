// byte_Width: 96

#define LIGHT(x) \
cbuffer Light : register(b##x##) \
{ \
  matrix light_vp; \
  float3 light_pos; \
  float3 camera_pos; \
  int2 __padding; \
}
