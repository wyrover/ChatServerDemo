#include <chat_server/ChatRoom.h>
#include <chat_server/ChatServer.h>
#include <chat_server/ChatUser.h>

using google::protobuf::MessageLite;

ChatRoom::ChatRoom(user_id_t creat_user_id, room_id_t room_id,
                   const std::string& room_name,
                   const std::string& room_passwd,
                   uint32_t max_user_count,
                   ChatServerPtr server) {
  creat_user_id_ = creat_user_id;
  room_id_ = room_id;
  room_name_ = room_name;
  room_passwd_ = room_passwd;
  max_user_count_ = max_user_count;
  server_ = server;
  users_.insert(creat_user_id);
}

ChatRoom::~ChatRoom() {}

void ChatRoom::BroadcastMessage(uint16_t type, const MessageLite &message) {
  for (user_id_t uid : users_) {
    ChatUserPtr user = server_->FindUser(uid);
    if (user != nullptr) {
      user->Write(type, message);
    } else {
      CS_LOG_ERROR("ChatRoom::BroadcastMessage: user not found");
    }
  }
}
