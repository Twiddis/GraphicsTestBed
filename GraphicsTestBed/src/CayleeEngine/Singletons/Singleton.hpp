#pragma once
#include <memory>

namespace CayleeEngine
{

template <typename T>
class Singleton
{
public:
  inline Singleton() = default;
  ~Singleton() = default;
  
  template <typename ...Args>
  static inline void Initialize(Args &&... args);

  static constexpr void Shutdown();
  static constexpr T* GetInstance();

private:
  static std::unique_ptr<T> sInstance;
};

}

#include "Singleton.inl"