#include "../InputLayouts/default.hlsl"
#include "../ConstantBuffers/EntityTransform.hlsl"

ENTITY_TRANSFORM(0);

VertexOut main(VertexIn vin)
{
  VertexOut output;

  matrix mv = mul(model, view);
  matrix mvp = mul(mv, projection);

  output.pos = mul(float4(vin.pos, 1.0f), mvp);
  output.normal = mul(mv, vin.normal);
  output.uv = vin.uv;

  return output;
}
