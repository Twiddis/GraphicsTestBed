// byte_Width: 192

#define CAMERA(x) \
cbuffer Camera : register(b##x##) \
{ \
  matrix model;      \
  matrix view;       \
  matrix projection; \
}