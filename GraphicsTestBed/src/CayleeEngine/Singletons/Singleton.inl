namespace CayleeEngine
{
template <typename T>
std::unique_ptr<T> Singleton<T>::sInstance = nullptr;

template <typename T>
template <typename ...Args>
inline void Singleton<T>::Initialize(Args&& ...args)
{
  err::AssertWarn(!sInstance.get(), "WARNING: Attempted to an alreaedy instantiated singleton!");

  if (!sInstance)
    sInstance = std::make_unique<T>(args...);
}

template<typename T>
constexpr void Singleton<T>::Shutdown()
{
  err::AssertWarn(sInstance.get(), "WARNING: Attempted to an destroy an uninitialized singleton!");

  sInstance.reset();
}

template <typename T>
constexpr T* Singleton<T>::GetInstance()
{
  return sInstance.get();
}

}