#include "FileWriter.h"
#include <iostream>

#include <LZ4/lz4.h>
#include "../RoseEngine/Trace/Trace.h"

// Check endianness of processor
union {
  uint32_t i;
  char c[4];
} bint = { 0x01020304 };
bool ntr::FileWriter::m_isLittleEndian = !(bint.c[0] == 1);

namespace ntr
{
FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
}

void FileWriter::WriteBlankBytes(size_t num_bytes)
{
  m_buffer.insert(m_buffer.end(), num_bytes, std::byte{0x0});
}

void FileWriter::WriteToFile(const std::string &filename)
{
  if (m_buffer.size() > LZ4_MAX_INPUT_SIZE) {
    traceE << "Max Input size Reached" << std::endl;
    return;
  }

    // Compress the bytes
  uint64_t compression_size = static_cast<uint64_t>(LZ4_compressBound(static_cast<int>(m_buffer.size())));
  char *compressed_buffer = new char[compression_size];

  compression_size = LZ4_compress_default(reinterpret_cast<const char*>(m_buffer.data()), compressed_buffer,
                                          static_cast<int>(m_buffer.size()), 
                                          static_cast<int>(compression_size));

  traceD << "Compressing data of size: " << m_buffer.size() << " to file: " << filename << " of size: " << compression_size << std::endl;

  std::ofstream output_filestream(filename, std::ofstream::binary);

  uint64_t uncompressed_size = static_cast<uint64_t>(m_buffer.size());
  output_filestream.write(reinterpret_cast<char*>(&uncompressed_size), sizeof(uint64_t));
  output_filestream.write(compressed_buffer, compression_size);
  output_filestream.close();

  delete[] compressed_buffer;
}

void FileWriter::PrintBuffer()
{
}

}