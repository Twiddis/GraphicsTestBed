#pragma once
#include "Systems/System.hpp"

namespace CayleeEngine::sys
{
class Input : public System
{
public:
  Input() = default;
  ~Input() = default;

  void Update(float dt);
private:
};
}