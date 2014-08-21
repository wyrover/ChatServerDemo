#include <chat_server/ChatUser.h>
#include <chat_server/Logger.h>
#include <chat_server/wapis.h>
#include <chat_server/ChatServerErrorCode.h>
#include <chat_server/Session.h>
#include <chat_server/ChatServer.h>
#include <chat_server/ChatRoom.h>
#include <cassert>

ChatUser::ChatUser(ChatServerPtr server)
  : server_(server) {
}

ChatUser::~ChatUser() {
}

bool ChatUser::Initial(SessionPtr session) {
  set_session(session);
  return true;
}

void ChatUser::set_session(SessionPtr session) { 
  session_ = session;
  std::weak_ptr<ChatUser> wself(shared_from_this());
  session_->set_on_message_cb([wself](SessionPtr session,
    uint16_t type,
    const void* body,
    int16_t size) -> bool {
    if (ChatUserPtr self = wself.lock()) {
      return self->HandleMessage(session, type, body, size);
    } else {
      return false;
    }
  });
  session_->set_on_close_cb([wself](SessionPtr session) {
    if (ChatUserPtr self = wself.lock()) {
      self->OnClose(session);
    }
  });
}

void ChatUser::Write(uint16_t type, const MessageLite &message) {
  if (session_ != nullptr) {
    session_->Write(type, message);
  }
}

bool ChatUser::HandleMessage(SessionPtr session,
                             uint16_t message_type,
                             const void *body,
                             int16_t size) {
  switch (message_type) {
  case MSG_USER_LOGOUT:
  {
    UserLogoutRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleUserLogoutRequest(message);
  }
  case MSG_CREAT_ROOM:
  {
    CreatRoomRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleCreatRoomRequest(message);
  }
  case MSG_ENTER_ROOM:
  {
    EnterRoomRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleEnterRoomRequest(message);
  }
  case MSG_GROUP_MSG:
  {
    GroupChatRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleGroupChatRequest(message);
  }
  case MSG_LEAVE_ROOM:
  {
    LeaveRoomRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleLeaveRoomRequest(message);
  }
    break;
  default:
    break;
  }
  return false;
}

void ChatUser::OnClose(SessionPtr session) {
  server_->remove_user(shared_from_this());
}

void ChatUser::SendEnterRoomResponse(int32_t error_code) {
  EnterRoomResponse response;
  response.set_error_code(error_code);
  Write(MSG_ENTER_ROOM, response);
}

bool ChatUser::HandleUserLogoutRequest(UserLogoutRequest& message) {
  return false;
}

bool ChatUser::HandleCreatRoomRequest(CreatRoomRequest& message) {
  CreatRoomResponse response;
  do 
  {
    if (server_->room_count() == server_->max_room_count()) {
      response.set_error_code(ENOROOMRESOURCE);
      break;
    }
    ChatRoomPtr room = server_->CreatRoom(user_id_, message.room_name(),
                                          message.room_passwd(),
                                          message.max_user_count());
    entered_rooms_.insert(room->room_id());
    response.set_error_code(ESUCCEED);
    response.set_room_id(room->room_id());
  } while (false);
  Write(MSG_CREAT_ROOM, response);
  return true;
}

bool ChatUser::HandleEnterRoomRequest(EnterRoomRequest& message) {
  ChatRoomPtr room = server_->FindRoom(message.room_id());
  do
  {
    if (!room) {
      SendEnterRoomResponse(EROOMNAMENOTEXIST);
      break;
    }
    if (room->is_user_exist(user_id_)) {
      SendEnterRoomResponse(EALREADYINTHISROOM);
      break;
    }
    if (room->user_count() == room->max_user_count() ) {
      SendEnterRoomResponse(EROOMFULL);
      break;
    }
    if (room->room_passwd() != message.room_passwd()) {
      SendEnterRoomResponse(EROOMPASSWDERR);
      break;
    }
    room->add_user(user_id_);
    entered_rooms_.insert(room->room_id());
    SendEnterRoomResponse(ESUCCEED);

    UserEnteredNtfy ntfy;
    ntfy.set_user_id(user_id_);
    ntfy.set_nick_name(user_id_);
    room->BroadcastMessage(MSG_USER_ENTERED, ntfy);
  } while (false);
  return true;
}

bool ChatUser::HandleLeaveRoomRequest(LeaveRoomRequest& message) {
  do {
    auto iter = entered_rooms_.find(message.room_id());
    if (iter == entered_rooms_.end()) {
      CS_LOG_DEBUG("You have not entered that room");
      break;
    }
    entered_rooms_.erase(iter);
    on_leave_(user_id_);
  } while (false);
  return true;
}

bool ChatUser::HandleGroupChatRequest(GroupChatRequest& message) {
  if (entered_rooms_.find(message.room_id()) == entered_rooms_.end()) {
    CS_LOG_ERROR("You have not entered that room");
    return true;
  }
  ChatRoomPtr room = server_->FindRoom(message.room_id());
  if (room != nullptr) {
    GroupChatNtfy ntfy;
    ntfy.set_room_id(message.room_id());
    ntfy.set_message_content(message.message_content());
    ntfy.set_sender_user_id(user_id_);
    room->BroadcastMessage(MSG_GROUP_MSG, ntfy);
  }
  return true;
}
