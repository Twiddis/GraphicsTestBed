#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace ntr
{
class FileWriter
{
public:
  FileWriter();
  ~FileWriter();

  template <typename T>
  inline void Write(const T &val, size_t num_bytes);

  void WriteBlankBytes(size_t num_bytes);

  void WriteToFile(const std::string &filename);
  void PrintBuffer();

private:
  static bool m_isLittleEndian;

  std::vector<std::byte> m_buffer;
};

template<typename T>
inline void FileWriter::Write(const T &val, size_t num_bytes)
{
  const std::byte *byte_start = reinterpret_cast<const std::byte*>(&val);
  m_buffer.insert(m_buffer.end(), byte_start, byte_start + num_bytes);
}
}