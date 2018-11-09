#pragma once
#include <vector>

#include "Keyframe/Keyframe.h"

namespace ntr
{
namespace AssetLoader
{

class Animation
{
public:
  Animation();
  Animation(int anim_ID) : m_animationID(anim_ID) {}
  ~Animation();

  int m_animationID;
  std::vector<Keyframe> m_keyFrames;
};

}
}