#ifndef CRAZY_NET_BYTE_ARRAY_H
#define CRAZY_NET_BYTE_ARRAY_H

#include "internal/common.h"
#include <cstdint>
#include <cstring>
#include <cassert>
#include <vector>
#include <type_traits>
#include <boost/utility.hpp>

NS_AVXER_BEGIN

class ByteArray : public boost::noncopyable {
public:
	enum class EndianType : int8_t {
		Little,
		Big
	};


  /**
   * @return the host's cpu endian type
   */
	static EndianType GetHostEndianType() {
		union {
			int i;
			char c;
		} u;
		u.i = 1;
    return u.c == 1 ? EndianType::Little : EndianType::Big;
	}


	/**
	 * @return whether the host's cpu is big endian or not
   * @see GetHostEndianType
	 */
	static bool IsBigEndian() {
    return EndianType::Big == GetHostEndianType();
	}


	/**
	 * @see IsBigEndian
	 */
	static bool IsNetworkEndian() {
		return IsBigEndian();
	}


  /** swap byte order */
  static int16_t swap(int16_t value) {
    return ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
  }


  /** swap byte order */
  static int32_t swap(int32_t value) {
    return ((value & 0xff) << 24) | ((value & 0xff00) << 8)
      | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24);
  }


  /** swap byte order */
  static int64_t swap(int64_t value) {
    int32_t low = (int32_t)(value & 0x00000000FFFFFFFFLL);
    int32_t high = (int32_t)((value & 0xFFFFFFFF00000000LL) >> 32);
    low = swap(low);
    high = swap(high);
    return (int64_t)high + (((int64_t)low) << 32);
  }


  /**
   * @return next read/write position 
   */
  uint32_t position() const {
    return position_;
  }


  /**
   * set next read/write position
   * @param pos
   */
  void set_position(uint32_t position) {
    if (position > data_.size()) {
      data_.resize(position);
    }
    position_ = position;
  }


	/**
	 * @param output_buffer
	 * @param length
	 * @return 
	 */
	void Read(void *output_buffer, uint32_t length) {
    if (!length) {
      throw std::logic_error("invalid parameter");
    }
    if (position_ + length > data_.size()) {
      throw std::overflow_error("read exceed eof");
    }
    std::memcpy(output_buffer, &data_[position_], length);
	  position_ += length;
	}

  int8_t ReadChar() {
    int8_t value;
    Read(&value, sizeof(value));
    return value;
  }

  uint8_t ReadUnsignedChar() {
    return static_cast<uint8_t>(ReadChar());
  }

  int16_t ReadShort() {
    int16_t value;
    Read(&value, sizeof(value));
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    return value;
  }

  uint16_t ReadUnsignedShort() {
    return static_cast<uint16_t>(ReadInt64());
  }

  int32_t ReadInt() {
    int32_t value;
    Read(&value, sizeof(value));
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    return value;
  }

  uint32_t ReadUnsignedInt() {
    return static_cast<uint32_t>(ReadInt64());
  }

  int64_t ReadInt64() {
    int64_t value;
    Read(&value, sizeof value);
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    return value;
  }

  uint64_t ReadUnsignedInt64() {
    return static_cast<uint64_t>(ReadInt64());
  }

	/**
	 * write start current postion, may overwrite data on current position.
	 * @param output_buffer
	 * @param length
	 */
  void Write(const void *input_buffer, uint32_t length) {
    if (!length) {
      return;
    }
    if (position_ + length > data_.size()) {
      data_.resize(position_ + length);
    }
    std::memcpy(&data_[position_], input_buffer, length);
    position_ += length;
  }

  ByteArray& WriteChar(int8_t value) {
    Write(&value, sizeof(value));
    return *this;
  }

  ByteArray& WriteUnsignedChar(uint8_t value) {
    return WriteChar(static_cast<int8_t>(value));
  }

  ByteArray& WriteShort(int16_t value) {
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    Write(&value, sizeof(value));
    return *this;
  }

  ByteArray& WriteUnsignedShort(uint16_t value) {
    return WriteShort(static_cast<int16_t>(value));
  }

  ByteArray& WriteInt(int32_t value) {
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    Write(&value, sizeof(value));
    return *this;
  }

  ByteArray& WriteUnsignedInt(uint32_t value) {
    return WriteInt(static_cast<int32_t>(value));
  }

  ByteArray& WriteInt64(int64_t value) {
    if (!IsNetworkEndian()) {
      value = swap(value);
    }
    Write(&value, sizeof(value));
    return *this;
  }

  ByteArray& WriteUnsignedInt64(uint64_t value) {
    return WriteInt64(static_cast<int64_t>(value));
  }

private:
	std::vector<uint8_t> data_;
	uint32_t position_;
};

NS_AVXER_END

#endif /* CRAZY_NET_BYTE_ARRAY_H */
