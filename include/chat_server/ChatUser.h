#ifndef CHAT_SERVER_USER_H
#define CHAT_SERVER_USER_H

#include "Macros.h"
#include "Session.h"
#include "ChatServerPacket.h"
#include <memory>
#include <set>
#include <string>
#include <boost/asio.hpp>

class ChatUser : public std::enable_shared_from_this<ChatUser> {
public:
  static ChatUserPtr Create(SessionPtr session, ChatServerPtr server,
                            boost::asio::io_service &ios) {
    ChatUserPtr user = std::make_shared<ChatUser>(server, ios);
    if (user && user->Initial(session)) {
      return user;
    }
    return nullptr;
  }

  ChatUser(ChatServerPtr server, boost::asio::io_service &ios);
  ~ChatUser();

  user_id_t user_id() const { return user_id_; }
  void set_user_id(user_id_t id) { user_id_ = id; }
  std::string nick_name() const { return nick_name_; }
  void set_nick_name(const std::string &name) { nick_name_ = name; }
  SessionPtr session() const { return session_; }
  void set_session(SessionPtr session);

  void Write(uint16_t type, const MessageLite &message);

  void add_to_room(room_id_t room_id) {
    auto iter = entered_rooms_.find(room_id);
    if (iter == entered_rooms_.end()) {
      entered_rooms_.insert(room_id);
    } else {
      CS_LOG_ERROR("already enter that room, room id = " << room_id);
    }
  }
  void remove_from_room(room_id_t room_id) {
    auto iter = entered_rooms_.find(room_id);
    if (iter != entered_rooms_.end()) {
      entered_rooms_.erase(iter);
    } else {
      CS_LOG_ERROR("have not enter that room, room id = " << room_id);
    }
  }

private:
  bool Initial(SessionPtr session);

  bool HandleMessage(SessionPtr session,
                     uint16_t message_type,
                     const void *body,
                     int16_t size);
  void OnClose(SessionPtr session, CloseReason reason);

  bool HandleUserLogoutRequest();
  bool HandleCreatRoomRequest(const CreatRoomRequest& message);
  bool HandleEnterRoomRequest(const EnterRoomRequest& message);
  bool HandleLeaveRoomRequest(const LeaveRoomRequest& message);
  bool HandleRoomChatRequest(const RoomChatRequest& message);
  
  bool HandlePing(const Ping& message);
  bool HandleKeepAliveRequest(const KeepAliveRequest& message);

private:
  SessionPtr session_;
  ChatServerPtr server_;
  user_id_t user_id_;
  std::string nick_name_;
  std::set<room_id_t> entered_rooms_;
  boost::asio::deadline_timer offline_timer_;
};

#endif /* CHAT_SERVER_USER_H */
