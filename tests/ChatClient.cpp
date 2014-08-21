#include "ChatClient.h"

using namespace boost::asio;

void ChatClient::Login(const std::string &login_name,
                       const std::string &login_passwd) {
  UserLoginRequest message;
  message.set_login_name(login_name);
  message.set_login_passwd(login_passwd);
  if (session_) {
    session_->Write(MSG_USER_LOGIN, message);
  }
}

void ChatClient::Logout() {
  UserLogoutRequest message;
  if (session_) {
    session_->Write(MSG_USER_LOGOUT, message);
  }
}

void ChatClient::CreatRoom(const std::string &room_name, const std::string& room_passwd, uint32_t max_user_count) {
  CreatRoomRequest message;
  message.set_room_name(room_name);
  message.set_room_passwd(room_passwd);
  message.set_max_user_count(max_user_count);
  if (session_) {
    session_->Write(MSG_CREAT_ROOM, message);
  }
}

void ChatClient::EnterRoom(const room_id_t& room_id, const std::string& room_passwd) {
  EnterRoomRequest message;
  message.set_room_id(room_id);
  message.set_room_passwd(room_passwd);
  if (session_) {
    session_->Write(MSG_ENTER_ROOM, message);
  }
}

void ChatClient::SendGroupMessage(const room_id_t& room_id, const void* data, uint32_t size) {
  GroupChatRequest message;
  message.set_room_id(room_id);
  message.set_message_content(data, size);
  if (session_) {
    session_->Write(MSG_GROUP_MSG, message);
  }
}

bool ChatClient::HandleMessage(SessionPtr session, uint16_t type, const void* body, int16_t size) {
  bool handled = false;
  bool parse_ok = false;
  switch (type) {
  case MSG_USER_LOGIN:
  {
    UserLoginResponse message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleUserLoginResponse(message);
    }
  }
    break;
  case MSG_CREAT_ROOM:
  {
    CreatRoomResponse message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleCreatRoomResponse(message);
    }
  }
    break;
  case MSG_ENTER_ROOM:
  {
    EnterRoomResponse message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleEnterRoomResponse(message);
    }
  }
    break;
  case MSG_LEAVE_ROOM:
    break;
  case MSG_USER_ENTERED:
  {
    UserEnteredNtfy message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleUserEntered(message);
    }
  }
    break;
  case MSG_USER_LEAVED:
  {
    UserLeavedNtfy message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleUserLeaved(message);
    }
  }
    break;
  case MSG_GROUP_MSG:
  {
    GroupChatNtfy message;
    if (parse_ok = message.ParseFromArray(body, size)) {
      handled = HandleGroupMessage(message);
    }
  }
    break;
  default:
    CS_LOG_ERROR("Unknown message type = " << type);
    break;
  }

  if (!parse_ok) {
    CS_LOG_ERROR("parse failed, type = " << type);
  }
  return handled;
}

void ChatClient::OnClose(SessionPtr session) {
  CS_LOG_DEBUG("ChatClient::OnClose");
}

bool ChatClient::HandleUserLoginResponse(const UserLoginResponse& message) {
  CS_LOG_DEBUG("HandleUserLoginResponse: error_code = " << message.error_code());
  return true;
}

bool ChatClient::HandleCreatRoomResponse(const CreatRoomResponse& message) {
  CS_LOG_DEBUG("HandleCreatRoomResponse: error_code = " << message.error_code());
  if (ESUCCEED == message.error_code()) {
    CS_LOG_DEBUG("room id = " << message.room_id());
  }
  return true;
}

bool ChatClient::HandleEnterRoomResponse(const EnterRoomResponse& message) {
  CS_LOG_DEBUG("HandleEnterRoomResponse: error_code = " << message.error_code());
  return true;
}

bool ChatClient::HandleUserEntered(const UserEnteredNtfy& message) {
  CS_LOG_DEBUG("HandleUserEntered: " << message.user_id());
  return true;
}

bool ChatClient::HandleUserLeaved(const UserLeavedNtfy& message) {
  CS_LOG_DEBUG("HandleUserLeaved: " << message.user_id());
  return true;
}

bool ChatClient::HandleGroupMessage(const GroupChatNtfy& message) {
  CS_LOG_DEBUG("[" << message.sender_user_id() << "]:" << message.message_content());
  return true;
}
