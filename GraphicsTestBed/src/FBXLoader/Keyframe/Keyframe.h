#pragma once
#include <fbxsdk.h>
#include <cstdint>

namespace ntr {
namespace AssetLoader {



struct Keyframe
{
  uint32_t m_time; // Time passed in milliseconds

  FbxDouble3 m_translation = FbxDouble3(0.0f, 0.0f, 0.0f);
  FbxDouble3 m_scale = FbxDouble3(1.0f, 1.0f, 1.0f);
  FbxDouble4 m_rotation = FbxDouble4(0.0f, 0.0f, 0.0f, 0.0f);

  inline bool operator==(const Keyframe &rhs)
  {
    if (!FbxEqual(m_translation, rhs.m_translation))
      return false;
    else if (!FbxEqual(m_scale, rhs.m_scale))
      return false;
    else if (!FbxEqual(m_rotation, rhs.m_rotation))
      return false;
    return true;
  }

  inline bool operator!=(const Keyframe &rhs)
  {
    return !(*this == rhs);
  }
};
}
}