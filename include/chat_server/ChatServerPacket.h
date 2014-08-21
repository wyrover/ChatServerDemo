#ifndef CHAT_SERVER_CHATSERVERPACKET_H
#define CHAT_SERVER_CHATSERVERPACKET_H

#include <cstdint>
#include <vector>
#include <memory>
#include <google/protobuf/message_lite.h>
#include <avxer/Buffer.h>
#include "ChatServerMessages.pb.h"

using google::protobuf::MessageLite;
using avxer::Buffer;

class PacketHeader {
public:
  PacketHeader() {
    this->length_ = 0;
    this->message_type_ = 0;
  }

  PacketHeader(uint16_t length, uint16_t message_type) {
    this->length_ = length;
    this->message_type_ = message_type;
  }

  uint16_t length() const {
    return length_;
  }

  uint16_t message_type() const {
    return message_type_;
  }

  int Serialize(Buffer &output_buffer) {
    output_buffer.writeUInt16BE(length_, 0);
    output_buffer.writeUInt16BE(message_type_, 2);
    return 0;
  }

  int Deserialize(Buffer &input_buffer) {
    length_ = input_buffer.readUInt16BE(0);
    message_type_ = input_buffer.readUInt16BE(2);
    return 0;
  }

private:
  uint16_t length_; // packet size include this field
  uint16_t message_type_;
};

#define PACKET_HEADER_LENGTH 4u

typedef std::shared_ptr<Buffer> BufferPtr;

inline BufferPtr SerializeMessage(const MessageLite &message,
                               uint16_t message_type) {
  const int body_size = message.ByteSize();
  PacketHeader header(body_size + PACKET_HEADER_LENGTH, message_type);
  std::shared_ptr<Buffer> buffer(new Buffer(header.length()));
  header.Serialize(*buffer);
  if (body_size > 0) {
    bool result = message.SerializeToArray(&(*buffer)[PACKET_HEADER_LENGTH],
                                           message.ByteSize());
    assert(result);
  }
  return buffer;
}

typedef uint32_t room_id_t;
typedef std::string user_id_t;

enum {
  MSG_USER_LOGIN = 1, /** 用户登录 */
  MSG_USER_LOGOUT,    /** 用户注销 */
  MSG_ADMIN_LOGIN,    /** 管理员登录 */
  MSG_ADMIN_LOGOUT,   /** 管理员注销 */
  MSG_GROUP_MSG,      /** 群消息 */
  MSG_SYS_MSG,        /** 系统消息 */
  MSG_PRIVATE_MSG,    /** 私聊消息 */
  MSG_KEEP_ALIVE,     /** 测试存活 */
  MSG_PING,           /** 测试网络延迟 */
  MSG_CREAT_ROOM,     /** 创建聊天室 */
  MSG_ENTER_ROOM,     /** 加入聊天室 */
  MSG_LEAVE_ROOM,     /** 退出聊天室 */
  MSG_USER_ENTERED,   /** 用户进入事件 */
  MSG_USER_LEAVED,    /** 用户退出事件 */
};

#endif /* CHAT_SERVER_CHATSERVERPACKET_H */
