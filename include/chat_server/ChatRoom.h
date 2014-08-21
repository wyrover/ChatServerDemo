#ifndef CHAT_SERVER_CHATROOM_H
#define CHAT_SERVER_CHATROOM_H

#include "Macros.h"
#include "ChatServerPacket.h"
#include <memory>
#include <string>
#include <set>
#include <assert.h>
#include <google/protobuf/message_lite.h>

class ChatRoom : public std::enable_shared_from_this<ChatRoom> {
public:
  static ChatRoomPtr create(user_id_t creat_user_id, room_id_t room_id,
                            const std::string& room_name,
                            const std::string& room_passwd,
                            uint32_t max_user_count,
                            ChatServerPtr server) {
    ChatRoomPtr room = std::make_shared<ChatRoom>(creat_user_id, room_id, room_name,
                                                  room_passwd, max_user_count, server);
    if (room && room->initial()) {
      return room;
    }
    return nullptr;
  }

  ChatRoom(user_id_t creat_user_id, room_id_t room_id,
           const std::string& room_name,
           const std::string& room_passwd,
           uint32_t max_user_count,
           ChatServerPtr server);
  ~ChatRoom();

  user_id_t creat_user_id() const { return creat_user_id_; }
  room_id_t room_id() const { return room_id_; }
  std::string room_name() const { return room_name_; }
  std::string room_passwd() const { return room_passwd_; }
  uint32_t max_user_count() const { return max_user_count_; }
  uint32_t user_count() const { return static_cast<uint32_t>(users_.size()); }
  void set_creat_user_id(user_id_t id) { creat_user_id_ = id; }
  void set_room_id(room_id_t id) { room_id_ = id; }
  void set_room_name(const std::string& name) { room_name_ = name; }
  void set_room_passwd(const std::string &passwd) { room_passwd_ = passwd; }

  bool is_user_exist(user_id_t uid) const {
    return users_.find(uid) != users_.end();
  }
  void add_user(user_id_t uid) {
    assert(!is_user_exist(uid));
    users_.insert(uid);
  }
  void remove_user(user_id_t uid) {
    assert(is_user_exist(uid));
    users_.erase(uid);
  }

  void BroadcastMessage(uint16_t type, const google::protobuf::MessageLite &message);

private:
  bool initial() {
    return true;
  }

private:
  ChatServerPtr server_;
  user_id_t creat_user_id_;
  room_id_t room_id_;
  std::string room_name_;
  std::string room_passwd_;
  uint32_t max_user_count_;
  std::set<user_id_t> users_;
};

#endif /* CHAT_SERVER_CHATROOM_H */
