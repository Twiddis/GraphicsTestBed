// byte_Width: 256

#define ENTITY_TRANSFORM(x) \
cbuffer EntityTransform : register(b##x##) \
{ \
  matrix model; \
  matrix view; \
  matrix projection; \
  matrix trans_inv_model; \
}
