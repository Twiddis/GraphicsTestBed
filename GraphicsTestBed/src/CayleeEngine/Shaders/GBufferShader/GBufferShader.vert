#include "../InputLayouts/default.hlsl"
#include "../ConstantBuffers/EntityTransform.hlsl"

ENTITY_TRANSFORM(0);

VertexOut main(VertexIn vin)
{
  VertexOut output;

  float4 world_pos = mul(float4(vin.pos, 1.0f), model);

  output.pos = mul(mul(world_pos, view), projection);
  output.world_pos = world_pos.xyz;
  //output.normal = vin.normal;
  output.normal = normalize(mul(float4(vin.normal, 0.0f), trans_inv_model).xyz);
  output.uv = vin.uv;

  return output;
}
