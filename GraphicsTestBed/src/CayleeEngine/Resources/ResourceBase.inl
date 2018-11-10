#include "ResourceBase.hpp"

namespace CayleeEngine::res
{
template <typename T> std::vector<ResourceID>
  ResourceBase<T>::sFreeIDs = std::vector<ResourceID>();

template <typename T> std::unordered_map<ResourceID, std::unique_ptr<T>>
  ResourceBase<T>::sResources = std::unordered_map<ResourceID, std::unique_ptr<T>>();

template <typename T>
inline ResourceID ResourceBase<T>::Create()
{
  ResourceID new_id = sResources.size();

  if (sFreeIDs.size()) {
    new_id = sFreeIDs.back();
    sFreeIDs.pop_back();
  }

  sResources.emplace(new_id, std::make_unique<T>());
}

 template<typename T>
inline T *ResourceBase<T>::Get(ResourceID id)
{
  auto res = sResources.find(id);
  if (res != sResources.end())
    return res.get();

  return nullptr;
}

template<typename T>
inline void ResourceBase<T>::Destroy()
{
  sResources.erase(mID);
  sFreeIDs.push_back(mID);
}
}