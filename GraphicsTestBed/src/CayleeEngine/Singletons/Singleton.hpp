#pragma once
#include <memory>

namespace CayleeEngine
{

template <typename T>
class Singleton
{
public:
  inline Singleton() = default;
  ~Singleton();
  
  inline static T* GetInstance();

private:
  static std::unique_ptr<T> mInstance;
};

}

#include "Singleton.inl"