#ifndef CHAT_SERVER_USER_H
#define CHAT_SERVER_USER_H

#include "Macros.h"
#include "ChatServerPacket.h"
#include <memory>
#include <set>
#include <string>
#include <boost/signals2.hpp>

using namespace com::avxer::chat;
using boost::signals2::connection;

class ChatUser : public std::enable_shared_from_this<ChatUser> {
public:
  typedef boost::signals2::signal<void(user_id_t)> on_leave_t;
  typedef on_leave_t::slot_type on_leave_slot_t;

  static ChatUserPtr Create(SessionPtr session, ChatServerPtr server) {
    ChatUserPtr user = std::make_shared<ChatUser>(server);
    if (user && user->Initial(session)) {
      return user;
    }
    return nullptr;
  }

  ChatUser(ChatServerPtr server);
  ~ChatUser();

  user_id_t user_id() const { return user_id_; }
  void set_user_id(user_id_t id) { user_id_ = id; }
  std::string nick_name() const { return nick_name_; }
  void set_nick_name(const std::string &name) { nick_name_ = name; }
  SessionPtr session() const { return session_; }
  void set_session(SessionPtr session);

  void Write(uint16_t type, const MessageLite &message);

  connection add_user_leave_listener(const on_leave_slot_t &slot) {
    return on_leave_.connect(slot);
  }

private:
  bool Initial(SessionPtr session);

  bool HandleMessage(SessionPtr session,
                     uint16_t message_type,
                     const void *body,
                     int16_t size);
  void OnClose(SessionPtr session);

  void SendEnterRoomResponse(int32_t error_code);

  bool HandleUserLogoutRequest(UserLogoutRequest& message);
  bool HandleCreatRoomRequest(CreatRoomRequest& message);
  bool HandleEnterRoomRequest(EnterRoomRequest& message);
  bool HandleLeaveRoomRequest(LeaveRoomRequest& message);
  bool HandleGroupChatRequest(GroupChatRequest& message);

private:
  SessionPtr session_;
  ChatServerPtr server_;
  user_id_t user_id_;
  std::string nick_name_;
  std::set<room_id_t> entered_rooms_;
  on_leave_t on_leave_;
};

#endif /* CHAT_SERVER_USER_H */
