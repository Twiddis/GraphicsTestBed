#pragma once
#include "Systems/System.hpp"
namespace CayleeEngine::sys
{
class Graphics : public System
{
public:
  Graphics();
  ~Graphics();

  void Update(float dt);

private:

};

}