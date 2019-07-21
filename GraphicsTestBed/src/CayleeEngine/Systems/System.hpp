#pragma once

namespace CayleeEngine
{
class System
{
public:
  System() : mIsActive(false) {}
  virtual ~System() {};

  virtual void StartFrame() = 0;
  virtual void Update(float dt) = 0;
  virtual void EndFrame() = 0;

  void constexpr Enable(bool flag = true) { mIsActive = flag; }
  void constexpr Disable(bool flag = true) { mIsActive = !flag; }
  
  bool constexpr IsEnabled() { return mIsActive; }

private:
  bool mIsActive;
};
}