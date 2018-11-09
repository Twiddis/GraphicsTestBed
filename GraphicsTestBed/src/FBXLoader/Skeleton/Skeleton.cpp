#include <Skeleton/Skeleton.h>

namespace ntr {
namespace AssetLoader {

Joint::Joint(const std::string &name, int parent_index, FbxNode *joint_node) : m_name(name), m_parentIndex(parent_index), m_jointNode(joint_node)
{
}

Joint::~Joint()
{
}

bool Joint::operator==(const std::string &name)
{
  if (m_name == name)
    return true;

  return false;
}

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

}
}