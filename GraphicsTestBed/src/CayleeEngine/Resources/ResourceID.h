#pragma once
class ResourceID
{
public:
  ResourceID() : mBytes() { std::memset(mBytes, 0, ID_SIZE); }
  ~ResourceID() = default;

  void GenerateNewID();

  ResourceID& operator=(const ResourceID &rhs);
  bool operator==(const ResourceID &rhs);

  static constexpr size_t ID_SIZE = 128;
  unsigned char mBytes[ID_SIZE];

private:
  static unsigned char GenerateByte();
};

namespace std
{
  template <>
  struct hash<ResourceID>
  {
    std::size_t operator()(const ResourceID &id) const
    {
      return hash<unsigned char>()(mBytes[0]);
    }
  }
}