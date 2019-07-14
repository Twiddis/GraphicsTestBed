#pragma once
#include "Resources/ResourceBase.hpp"

namespace CayleeEngine::res
{
class Mesh : public ResourceBase<Mesh>
{
public:
  Mesh();
  ~Mesh();
  

  enum VertexAttributeType
  {
    Position,
    Normal,
    UV,
    COUNT
  };

  template <typename T>
  inline void AppendData(VertexAttributeType attribute_type, 
                        typename const std::vector<T>::iterator &begin, typename const std::vector<T>::iterator &end);

  template <typename T>
  inline void AppendData(VertexAttributeType attribute_type, const T &element);

private:
  std::vector<Vec3> mPositions;
  std::vector<Vec3> mNormals;
  std::vector<Vec2> mUVs;
  
};


template <typename T>
inline void Mesh::AppendData(VertexAttributeType attribute_type, 
                             typename const std::vector<T>::iterator &begin, typename const std::vector<T>::iterator &end)
{
  switch (attribute_type)
  {
  case Position:
    mPositions.insert(mPositions.end(), begin, end);
    break;
  case Normal:
    mNormals.insert(mNormals.end(), begin, end);
    break;
  case UV:
    mUVs.insert(mUVs.end(), begin, end);
    break;
  default:
  }
}
template<typename T>
inline void Mesh::AppendData(VertexAttributeType attribute_type, const T & element)
{
  switch (attribute_type)
  {
  case Position:
    mPositions.push_back(element);
    break;
  case Normal:
    mNormals.push_back(element);
    break;
  case UV:
    mUVs.push_back(element);
    break;
  default:
  }
}
}