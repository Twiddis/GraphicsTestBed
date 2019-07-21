#include "../InputLayouts/default.hlsl"

VertexOut main(VertexIn vin)
{
  VertexOut output;

  output.pos = float4(vin.pos, 1);
  output.normal = vin.normal;
  output.uv = vin.uv;

  return output;
}