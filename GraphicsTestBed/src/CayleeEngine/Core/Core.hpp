#pragma once
#include "Systems/System.hpp"

namespace CayleeEngine
{
class Core
{
public:
  Core();
  ~Core();
  
  void StartSystem(std::unique_ptr<System> &system);

  void Run();
  void Shutdown();

private:

  std::vector<std::unique_ptr<System>> mSystems;
};

}