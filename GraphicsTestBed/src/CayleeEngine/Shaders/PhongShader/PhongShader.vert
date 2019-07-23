// Neat little trick found here:
// https://gamedev.stackexchange.com/questions/98283/how-do-i-draw-a-full-screen-quad-in-directx-11

struct FSQ_Vertex
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};

	// This is meant to be called with a regular non-indexed draw call with
	// 4 vertices as input.
	// Must also be called with primitive topology set to TRIANGLESTRIP
FSQ_Vertex main(uint vertex_id : SV_VERTEXID)
{
	FSQ_Vertex output;

		// Creates the following uv's:
		// ID   (u, v)
		// ------------------------
		// 0    (0, 0)
		// 1    (1, 0)
		// 2    (0, 1)
		// 3    (1, 1)

  // 0b00 = -1, -1
  // 0b01 =  1, -1
  // 0b10 = -1,  1
  // 0b11 =  1,  1
  output.pos = float4(
    ((vertex_id & 1) -.5f) * 2, //x
    ((vertex_id >> 1) -.5f) * 2, //y
    0, 1
  );

  output.uv = float2(
    vertex_id & 1,  //u
    !(vertex_id >> 1)  //v
  );

	return output;
}
