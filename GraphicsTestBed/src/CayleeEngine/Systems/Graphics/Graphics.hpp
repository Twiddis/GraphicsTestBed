#pragma once
#include "Systems/System.hpp"
namespace CayleeEngine::sys
{
class Graphics : public System
{
public:
  Graphics();
  ~Graphics();

  void StartFrame();
  void Update(float dt);
  void EndFrame();

private:

};

}