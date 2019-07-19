#pragma once
#include "ResourceID.h"

namespace CayleeEngine::res
{

/*!*****************************************************************************
  Lowest level of resource. Contains all common functionality that
  does not require specific typename information of instatiated types.
*******************************************************************************/
class ResourceInterface
{
public:
  ResourceInterface() {};
  virtual ~ResourceInterface() {};

private:
};

/*!*****************************************************************************
  Base class for resources. Statically contains templated container.
*******************************************************************************/
template <typename T>
class ResourceBase : public ResourceInterface
{
public:
  ResourceBase();
  virtual ~ResourceBase();
  
  class Key
  {
  public:
    Key(const ResourceID &id) : mID(id) {}
    Key(const Key &rhs) : mID(rhs.mID) {}

    Key& operator=(const Key& rhs) { mID = rhs.mID; }
    operator ResourceID() const { return Key(ResourceID); }
    
    T* operator->() { return T::Get(mID); }
     
  private:
    ResourceID mID;
  };

  template <typename... Args>
  inline static ResourceID Create(Args&&...args);

protected:
  inline static T* Get(ResourceID id);
  inline void Destroy();

private:
  static std::unordered_map<ResourceID, std::unique_ptr<T>> sResources;
};

} // CayleeEngine::res

#include "ResourceBase.inl"