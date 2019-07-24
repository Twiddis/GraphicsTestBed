#include "precompiled.hpp"
#include "Camera.hpp"

namespace CayleeEngine::res
{
  Camera::Camera() : mPosition(0.0f, 0.0f, 0.0f),
                     mScale(1920, 1080),
                     mUp(0.0f, 1.0f, 0.0f),
                     mRight(1.0f, 0.0f, 0.0f),
                     mPitch(0.0f), mYaw(0.0f)
  {
    
  }

  Camera::~Camera() {}

  void Camera::GenerateViewMatrix()
  {
    mViewMatrix = Mat::CreateTranslation(mPosition).Invert() * Mat(mRight, mUp, mRight.Cross(mUp)).Invert();
  }

  void Camera::GenerateProjectionMatrix()
  {
    mProjectionMatrix = Mat::CreatePerspectiveFieldOfView(3.14f * mScale.y / mScale.x * 0.8f, mScale.x / mScale.y, 0.1f, 100.0f);
  }

  void Camera::RotateCamera(float yaw, float pitch)
  {
    mYaw += yaw;
    mPitch += pitch;

    Mat rotation = Mat::CreateFromYawPitchRoll(mYaw, mPitch, 0);

    mRight = Vec3::Transform(RIGHT, rotation);
    mUp = Vec3::Transform(UP, rotation);
  }

}