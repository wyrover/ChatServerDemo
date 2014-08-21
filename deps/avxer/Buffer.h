#ifndef AVXER_BUFFER_H
#define AVXER_BUFFER_H

#include "internal/common.h"
#include "utils.h"
#include <cstdint>
#include <cstring>
#include <cassert>
#include <memory>

NS_AVXER_BEGIN

/**
 * A buffer that has fixed size.
 * LE means Little Endian, BE means Big Endian
 */
class Buffer {
public:
  Buffer(size_t size) : size_(size), buffer_(new uint8_t[size]) {
  }

  Buffer(const Buffer &rhs) : size_(rhs.size_) {
    buffer_.reset(new uint8_t[size_]);
    std::memcpy(buffer_.get(), rhs.buffer_.get(), size_);
  }

  Buffer& operator=(const Buffer &rhs) {
    if (&rhs != this) {
      size_ = rhs.size_;
      buffer_.reset(new uint8_t[size_]);
      std::memcpy(buffer_.get(), rhs.buffer_.get(), size_);
    }
    return *this;
  }

  Buffer(Buffer &&rhs) : size_(rhs.size_), buffer_(std::move(rhs.buffer_)) {}

  Buffer& operator=(Buffer &&rhs) {
    size_ = rhs.size_;
    buffer_ = std::move(rhs.buffer_);
    return *this;
  }

  size_t length() const {
    return size_;
  }

  uint8_t readUInt8(size_t offset) {
    return buffer_[offset];
  }

  uint16_t readUInt16LE(size_t offset) {
    return read<uint16_t>(offset, EndianType::LittleEndian);
  }

  uint16_t readUInt16BE(size_t offset) {
    return read<uint16_t>(offset, EndianType::BigEndian);
  }

  uint32_t readUInt32LE(size_t offset) {
    return read<uint32_t>(offset, EndianType::LittleEndian);
  }

  uint32_t readUInt32BE(size_t offset) {
    return read<uint32_t>(offset, EndianType::BigEndian);
  }

  uint64_t readUInt64LE(size_t offset) {
    return read<uint64_t>(offset, EndianType::LittleEndian);
  }

  uint64_t readUInt64BE(size_t offset) {
    return read<uint64_t>(offset, EndianType::BigEndian);
  }

  int8_t readInt8(size_t offset) {
    return readUInt8(offset);
  }

  int16_t readInt16LE(size_t offset) {
    return readUInt16LE(offset);
  }

  int16_t readInt16BE(size_t offset) {
    return readUInt16BE(offset);
  }
  
  int32_t readInt32LE(size_t offset) {
    return readUInt32LE(offset);
  }
  
  int32_t readInt32BE(size_t offset) {
    return readUInt32BE(offset);
  }

  int64_t readInt64LE(size_t offset) {
    return readUInt64LE(offset);
  }

  int64_t readInt64BE(size_t offset) {
    return readUInt64BE(offset);
  }
  
  float readFloatLE(size_t offset) {
    return read<float>(offset, EndianType::LittleEndian);
  }
  
  float readFloatBE(size_t offset) {
    return read<float>(offset, EndianType::BigEndian);
  }
  
  double readDoubleLE(size_t offset) {
    return read<double>(offset, EndianType::LittleEndian);
  }
  
  double readDoubleBE(size_t offset) {
    return read<double>(offset, EndianType::BigEndian);
  }

  Buffer& writeUInt8(uint8_t value, size_t offset) {
    buffer_[offset] = value;
    return *this;
  }
  
  Buffer& writeUInt16LE(uint16_t value, size_t offset) {
    write(value, offset, EndianType::LittleEndian);
    return *this;
  }
  
  Buffer& writeUInt16BE(uint16_t value, size_t offset) {
    write(value, offset, EndianType::BigEndian);
    return *this;
  }
  
  Buffer& writeUInt32LE(uint32_t value, size_t offset) {
    write(value, offset, EndianType::LittleEndian);
    return *this;
  }
  
  Buffer& writeUInt32BE(uint32_t value, size_t offset) {
    write(value, offset, EndianType::BigEndian);
    return *this;
  }

  Buffer& writeUInt64LE(uint64_t value, size_t offset) {
    write(value, offset, EndianType::LittleEndian);
    return *this;
  }

  Buffer& writeUInt64BE(uint64_t value, size_t offset) {
    write(value, offset, EndianType::BigEndian);
    return *this;
  }
  
  Buffer& writeInt8(int8_t value, size_t offset) {
    return writeUInt8(value, offset);
  }
  
  Buffer& writeInt16LE(int16_t value, size_t offset) {
    return writeUInt16LE(value, offset);
  }
  
  Buffer& writeInt16BE(int16_t value, size_t offset) {
    return writeUInt16BE(value, offset);
  }
  
  Buffer& writeInt32LE(int32_t value, size_t offset) {
    return writeUInt32LE(value, offset);
  }
  
  Buffer& writeInt32BE(int32_t value, size_t offset) {
    return writeUInt32BE(value, offset);
  }

  Buffer& writeInt64LE(int32_t value, size_t offset) {
    return writeUInt64LE(value, offset);
  }

  Buffer& writeInt64BE(int32_t value, size_t offset) {
    return writeUInt64BE(value, offset);
  }
  
  Buffer& writeFloatLE(float value, size_t offset) {
    write(value, offset, EndianType::LittleEndian);
    return *this;
  }
  
  Buffer& writeFloatBE(float value, size_t offset) {
    write(value, offset, EndianType::BigEndian);
    return *this;
  }
  
  Buffer& writeDoubleLE(double value, size_t offset) {
    write(value, offset, EndianType::LittleEndian);
    return *this;
  }
  
  Buffer& writeDoubleBE(double value, size_t offset) {
    write(value, offset, EndianType::BigEndian);
    return *this;
  }
  
  uint8_t operator[](size_t offset) const {
    return buffer_[offset];
  }

  uint8_t& operator[](size_t offset) {
    return buffer_[offset];
  }

  void fill(uint8_t value, size_t offset, size_t end) {
    assert(offset < size_ && offset < end);
    std::memset(&buffer_[offset], value, end - offset);
  }

  void read(size_t offset, void *buffer, size_t size) {
    std::memcpy(buffer, &buffer_[offset], size);
  }

  void write(const void *buffer, size_t size, size_t offset) {
    std::memcpy(&buffer_[offset], buffer, size);
  }

private:
  template<typename T>
  T read(size_t offset, EndianType endian_type) {
    T value = *reinterpret_cast<T*>(&buffer_[offset]);
    if (endian_type != GetHostEndianType()) {
      swap(&value, sizeof(value));
    }
    return value;
  }

  template<typename T>
  void write(T value, size_t offset, EndianType endian_type) {
    if (endian_type != GetHostEndianType()) {
      swap(&value, sizeof(value));
    }
    std::memcpy(&buffer_[offset], &value, sizeof(value));
  }

  void check_bound(size_t offset, size_t size) {
    if (offset + size >= size_) {
      throw std::overflow_error("reach EOF");
    }
  }

private:
  size_t size_;
  std::unique_ptr<uint8_t[]> buffer_;
};

NS_AVXER_END

#endif /* AVXER_BUFFER_H */
