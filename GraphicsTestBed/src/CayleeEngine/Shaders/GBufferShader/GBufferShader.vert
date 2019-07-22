#include "../InputLayouts/default.hlsl"
#include "../ConstantBuffers/EntityTransform.hlsl"

ENTITY_TRANSFORM(0);

VertexOut main(VertexIn vin)
{
  VertexOut output;

  matrix mv = mul(view, model);
  matrix mvp = mul(projection, mv);

  output.pos = mul(mv, float4(vin.pos, 1.0f));
  output.normal = mul(mvp, vin.normal).xyz;
  output.uv = vin.uv;

  return output;
}
