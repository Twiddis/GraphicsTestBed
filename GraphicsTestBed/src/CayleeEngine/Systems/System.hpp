#pragma once

namespace CayleeEngine
{
class System
{
public:
  System() : mIsActive(false) {}

  virtual ~System() {};

  virtual void Initialize() = 0;
  virtual void Update(float dt) = 0;
  virtual void Clean();

  void Enable(bool flag = true) { mIsActive = flag; }
  void Disable(bool flag = true) { mIsActive = !flag; }

private:
  bool mIsActive;
};
}