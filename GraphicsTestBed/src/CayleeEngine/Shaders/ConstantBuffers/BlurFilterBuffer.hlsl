// byte_Width: 544

#define BLUR_FILTER_BUFFER(x) \
cbuffer BlurFilterBuffer : register(b##x##) \
{ \
  float blur_weights[33]; \
  int blur_output_size; \
  int blur_input_size; \
  int2 __padding_; \
}
