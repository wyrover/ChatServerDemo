#ifndef AVXER_UTILS_H
#define AVXER_UTILS_H

#include "internal/common.h"
#include <cstdint>

NS_AVXER_BEGIN


enum class EndianType : int8_t {
  LittleEndian,
  BigEndian
};

inline EndianType GetHostEndianType() {
  union {
    int i;
    char c;
  } u;
  u.i = 1;
  return u.c == 1 ? EndianType::LittleEndian : EndianType::BigEndian;
}

inline bool IsBigEndian() {
  return EndianType::BigEndian == GetHostEndianType();
}

inline bool IsLittleEndian() {
  return !IsBigEndian();
}

inline uint16_t swap(uint16_t value) {
  return ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
}

inline uint32_t swap(uint32_t value) {
  return ((value & 0xff) << 24) | ((value & 0xff00) << 8)
    | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24);
}

inline uint64_t swap(uint64_t value) {
  uint32_t low = (uint32_t)(value & 0x00000000FFFFFFFFLL);
  uint32_t high = (uint32_t)((value & 0xFFFFFFFF00000000LL) >> 32);
  low = swap(low);
  high = swap(high);
  return (uint64_t)high + (((uint64_t)low) << 32);
}

inline void swap(void *bytes, size_t size) {
  for (size_t i = 0; i < size / 2; ++i) {
    char tmp = reinterpret_cast<char*>(bytes)[i];
    reinterpret_cast<char*>(bytes)[i] = reinterpret_cast<char*>(bytes)[size - i - 1];
    reinterpret_cast<char*>(bytes)[size - i - 1] = tmp;
  }
}

NS_AVXER_END

#endif /* AVXER_UTILS_H */
