#pragma once
class ResourceID
{
public:
  ResourceID() : mBytes() { std::memset(mBytes, 0, ID_SIZE); }
  ~ResourceID() = default;

  void GenerateNewID();

  ResourceID& operator=(const ResourceID &rhs);
  bool operator==(const ResourceID &rhs);

private:
  static constexpr size_t ID_SIZE = 128;

  static unsigned char GenerateByte();
  unsigned char mBytes[ID_SIZE];
};
