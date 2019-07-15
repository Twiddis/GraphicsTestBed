#include "ResourceBase.hpp"

namespace CayleeEngine::res
{
template <typename T> std::unordered_map<ResourceID, std::unique_ptr<T>>
  ResourceBase<T>::sResources = std::unordered_map<ResourceID, std::unique_ptr<T>>();

template <typename T>
inline ResourceBase<T>::ResourceBase() { }
template <typename T>
inline ResourceBase<T>::~ResourceBase() { }

template <typename T>
template <typename... Args>
inline const ResourceID& ResourceBase<T>::Create(Args&&... args)
{
  ResourceID resource_id;
  resource_id.GenerateNewID();

  sResources.emplace(resource_id, std::make_unique<T>(args...));
  return resource_id;
}

template <typename T>
inline T* ResourceBase<T>::Get(ResourceID id)
{
  auto res = sResources.find(id);
  if (res != sResources.end())
    return res->second.get();
  else
    return nullptr;
}

template <typename T>
inline void ResourceBase<T>::Destroy()  
{
  sResources.erase(mID);
}



}