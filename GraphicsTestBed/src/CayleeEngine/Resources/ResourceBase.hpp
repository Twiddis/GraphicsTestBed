#pragma once

namespace CayleeEngine::res
{
typedef uint64_t ResourceID;

class ResourceInterface
{
public:
  ResourceInterface() : mID(0) {};
  virtual ~ResourceInterface() = 0;

protected:
  ResourceID mID;
};

template <typename T>
class ResourceBase : public ResourceInterface
{
public:

  inline static ResourceID Create();
  inline static T* Get(ResourceID id);
  inline void Destroy();

private:
  static std::vector<ResourceID> sFreeIDs;
  static std::unordered_map<ResourceID, std::unique_ptr<T>> sResources;
};

}

#include "ResourceBase.inl"