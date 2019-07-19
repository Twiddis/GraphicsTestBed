#include "precompiled.hpp"
#include "ResourceID.h"

void ResourceID::GenerateNewID()
{
  std::generate(mBytes, mBytes + ID_SIZE, GenerateByte);
}

ResourceID& ResourceID::operator=(const ResourceID & rhs)
{
  std::memcpy(mBytes, rhs.mBytes, ID_SIZE);
  return *this;
}

bool ResourceID::operator==(const ResourceID &rhs) const
{
  return std::equal(mBytes, mBytes + ID_SIZE, rhs.mBytes);
}

unsigned char ResourceID::GenerateByte()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return static_cast<unsigned char>(dis(gen));
}
