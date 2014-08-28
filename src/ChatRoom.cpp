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

void ChatRoom::HandleEnterRoomRequest(ChatUserPtr user) {
  add_user(user->user_id());
  user->add_to_room(room_id_);
  UserEnteredNtfy ntfy;
  ntfy.set_user_id(user->user_id());
  ntfy.set_nick_name(user->user_id());
  BroadcastMessage(MSG_USER_ENTERED, ntfy);
}

void ChatRoom::HandleRoomChatRequest(ChatUserPtr user, const RoomChatRequest& message) {
  GroupChatNtfy ntfy;
  ntfy.set_room_id(room_id_);
  ntfy.set_message_content(message.message_content());
  ntfy.set_sender_user_id(user->user_id());
  BroadcastMessage(MSG_ROOM_MSG, ntfy);
}

void ChatRoom::HandleLeaveRoomRequest(ChatUserPtr user) {
  auto iter = users_.find(user->user_id());
  if (iter == users_.end()) {
    CS_LOG_ERROR("ChatRoom::HandleLeaveRoomRequest: user not in this room");
    return;
  }
  UserLeavedNtfy ntfy;
  ntfy.set_user_id(user->user_id());
  BroadcastMessage(MSG_USER_LEAVED, ntfy);
  users_.erase(iter);
  user->remove_from_room(room_id_);

  if (users_.empty()) {
    CS_LOG_DEBUG("all user has left, room will be dismissed");
    server_->remove_room(room_id_);
  }
}

void ChatRoom::HandleUserLogoutRequest(ChatUserPtr user) {
  HandleLeaveRoomRequest(user);
}
