#pragma once
#include "Systems/System.hpp"
namespace CayleeEngine
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