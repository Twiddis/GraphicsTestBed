#pragma once
#include "ResourceID.h"

// NOTE:
// Consider making every resource bindable, buildable, and check if built.

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
    Key() : mID() {}
    Key(const ResourceID &id) : mID(id) {}
    Key(const Key &rhs) : mID(rhs.mID) {}

    Key& operator=(const Key& rhs) { mID = rhs.mID; return *this; }
    
    operator ResourceID() const { return mID; }
    operator bool() const { return T::Get(mID) ? true : false; }

    bool IsValid() const { return T::Get(mID) ? true : false; }

    T* operator->() { return T::Get(mID); }

    inline void Destroy() { T::sResources.erase(mID); }

  private:
    ResourceID mID;
  };

  template <typename... Args>
  inline static ResourceID Create(Args&&...args);

  static std::unordered_map<ResourceID, std::unique_ptr<T>>&
    GetResources() { return sResources; }

  ResourceID mID;

protected:
  inline static T* Get(ResourceID id);
  static std::unordered_map<ResourceID, std::unique_ptr<T>> sResources;

private:
};

} // CayleeEngine::res

#include "ResourceBase.inl"