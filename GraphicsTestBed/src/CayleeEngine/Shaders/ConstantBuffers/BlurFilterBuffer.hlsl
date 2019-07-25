// byte_Width: 96

#define BLUR_FILTER_BUFFER(x) \
cbuffer BlurFilterBuffer : register(b##x##) \
{ \
  float blur_weights[5]; \
  int blur_output_size; \
  int blur_input_size; \
  int2 __padding_; \
}
