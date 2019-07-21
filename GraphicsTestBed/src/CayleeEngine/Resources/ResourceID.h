#pragma once
class ResourceID
{
public:
  ResourceID() : mBytes() { std::memset(mBytes, 0, ID_SIZE); }
  ~ResourceID() = default;

  void GenerateNewID();

  ResourceID& operator=(const ResourceID &rhs);
  bool operator==(const ResourceID &rhs) const;

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
      uint32_t hash_value = 0;

      for (size_t i = 0; i < ResourceID::ID_SIZE; i += sizeof(uint32_t))
      {
        uint32_t bytes = *reinterpret_cast<const uint32_t*>(id.mBytes + i);
        hash_value = (hash_value ^ (hash<uint32_t>()(bytes) << 1) >> 1);
      }

      return hash_value;
    }
  };
}