#include "Singleton.hpp"

namespace CayleeEngine
{
template <typename T>
std::unique_ptr<T> Singleton<T>::mInstance = std::make_unique<T>();

template<typename T>
inline Singleton<T>::~Singleton()
{
}

template <typename T>
inline T* Singleton<T>::GetInstance()
{
  return mInstance.get();
}

}