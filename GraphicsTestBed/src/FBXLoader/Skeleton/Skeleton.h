#pragma once
#include <vector>
#include <string>
#include <fbxsdk.h>

#include "Animation/Animation.h"

namespace ntr {
namespace AssetLoader {

struct BlendingWeight
{
  uint32_t joint_index;
  double_t blend_weight;
};

class Joint
{
public:
  Joint(const std::string &name, int parent_index, FbxNode *joint_node);
  ~Joint();

  std::string m_name;
  uint32_t m_parentIndex;

  bool operator==(const std::string &name);
  FbxMatrix m_globalBindPoseMatrix;
  FbxNode *m_jointNode;

  std::vector<Animation> m_animations;
};

class Skeleton
{
public:
  Skeleton();
  ~Skeleton();

  std::vector<Joint> m_joints;
};
}

}