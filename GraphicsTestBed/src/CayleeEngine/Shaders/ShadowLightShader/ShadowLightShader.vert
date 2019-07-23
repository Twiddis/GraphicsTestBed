#include "../InputLayouts/default.hlsl"
#include "../ConstantBuffers/EntityTransform.hlsl"

ENTITY_TRANSFORM(0);

float4 main(VertexIn vin) : SV_POSITION
{
  float4 pos = float4(vin.pos, 1.0f);

  float4 world_pos = mul(pos, model);
  float4 view_pos = mul(world_pos, view);
  float4 projection_pos = mul(view_pos, projection);

  return projection_pos;
}
