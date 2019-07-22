#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class Camera : public ResourceBase<Camera>
{
public:
  Camera();
  ~Camera();

  void GenerateViewMatrix();
  void GenerateProjectionMatrix();

  constexpr Mat& GetViewMatrix() { return mViewMatrix; };
  constexpr Mat& GetProjectionMatrix() { return mProjectionMatrix; }
  
  inline void PanRight(float scale);
  inline void PanUp(float scale);

  inline void ZoomIn(float scale);

  void RotateCamera(float yaw, float pitch);

private:
  Vec3 mPosition;
  Vec2 mScale;
  Vec3 mUp;
  Vec3 mRight;

  float mPitch;
  float mYaw;

  Mat mViewMatrix;
  Mat mProjectionMatrix;

  static constexpr Vec3 RIGHT = Vec3(1.0f, 0.0f, 0.0f);
  static constexpr Vec3 UP = Vec3(0.0f, 1.0f, 0.0f);

};

void inline Camera::PanRight(float scale)
{
  mPosition += mRight * scale;
}

void inline Camera::PanUp(float scale)
{
  mPosition += mUp * scale;
}

void inline Camera::ZoomIn(float scale)
{
  mPosition += mUp.Cross(mRight) * scale;
}


} 