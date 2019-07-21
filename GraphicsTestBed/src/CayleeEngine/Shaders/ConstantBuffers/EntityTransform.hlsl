// byte_Width: 192

#define ENTITY_TRANSFORM(x) \
cbuffer EntityTransform : register(b##x##) \
{ \
  matrix model;      \
  matrix view;       \
  matrix projection; \
}