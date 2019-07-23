// byte_Width: 32

#define LIGHT(x) \
cbuffer Light : register(b##x##) \
{ \
  float4 light_color;    \
  float3 light_position; \
  float light_radius;    \
}
