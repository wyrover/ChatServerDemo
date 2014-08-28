#include <chat_server/ChatUser.h>
#include <chat_server/Logger.h>
#include <chat_server/wapis.h>
#include <chat_server/ChatServerErrorCode.h>
#include <chat_server/ChatServer.h>
#include <chat_server/ChatRoom.h>
#include <cassert>

ChatUser::ChatUser(ChatServerPtr server, boost::asio::io_service &ios)
  : server_(server), offline_timer_(ios) {
}

ChatUser::~ChatUser() {
  CS_LOG_DEBUG("ChatUser destroyed");
}

bool ChatUser::Initial(SessionPtr session) {
  set_session(session);
  return true;
}

void ChatUser::set_session(SessionPtr session) {
  session_ = session;
  if (session_ != nullptr) {
    boost::system::error_code ec;
    offline_timer_.cancel(ec);
    if (ec) {
      CS_LOG_ERROR("offline_timer_ cancel failed: " << ec.message());
    }
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
    session_->set_on_close_cb([wself](SessionPtr session, CloseReason reason) {
      if (ChatUserPtr self = wself.lock()) {
        self->OnClose(session, reason);
      }
    });
  }
}

void ChatUser::Write(uint16_t type, const MessageLite &message) {
  SessionPtr session = session_;
  if (session != nullptr) {
    session->Write(type, message);
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
    return HandleUserLogoutRequest();
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
  case MSG_ROOM_MSG:
  {
    RoomChatRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleRoomChatRequest(message);
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
  case MSG_PING:
  {
    Ping message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandlePing(message);
  }
    break;
  case MSG_KEEP_ALIVE:
  {
    KeepAliveRequest message;
    if (!message.ParseFromArray(body, size)) {
      break;
    }
    return HandleKeepAliveRequest(message);
  }
    break;
  default:
    CS_LOG_ERROR("unknown message type");
    break;
  }
  CS_LOG_ERROR("message parse failed");
  return false;
}

void ChatUser::OnClose(SessionPtr session, CloseReason reason) {
  assert(reason != CloseReason::UNKNOWN);
  const bool is_offline = (CloseReason::READEOF == reason
    || CloseReason::NORESPONSE == reason);
  const uint32_t wait = server_->conf().offline_wait_seconds();
  if (is_offline && wait > 0) {
    CS_LOG_DEBUG("user offline");
    offline_timer_.expires_from_now(boost::posix_time::seconds(wait));
    auto self(shared_from_this());
    offline_timer_.async_wait([self](const boost::system::error_code& ec) {
      CS_LOG_DEBUG("user offline time out");
      self->HandleUserLogoutRequest();
      self->server_->remove_user(self);
    });
  } else {
    HandleUserLogoutRequest();
    server_->remove_user(shared_from_this());
  }
  set_session(nullptr);
}

bool ChatUser::HandleUserLogoutRequest() {
  auto iter = entered_rooms_.begin();
  while (iter != entered_rooms_.end()) {
    const room_id_t room_id = *iter++;
    ChatRoomPtr room = server_->FindRoom(room_id);
    if (room != nullptr) {
      room->HandleUserLogoutRequest(shared_from_this());
    } else {
      CS_LOG_ERROR("ChatUser::HandleUserLogoutRequest: room not found");
    }
  }
  return false;
}

bool ChatUser::HandleCreatRoomRequest(const CreatRoomRequest& message) {
  CreatRoomResponse response;
  do 
  {
    if (server_->room_count() == server_->max_room_count()) {
      response.set_error_code(ENOROOMRESOURCE);
      break;
    }
    ChatRoomPtr room = server_->CreatRoom(user_id_,
                                          message.room_name(),
                                          message.room_passwd(),
                                          message.max_user_count());
    add_to_room(room->room_id());
    response.set_error_code(ESUCCEED);
    response.set_room_id(room->room_id());
  } while (false);
  Write(MSG_CREAT_ROOM, response);
  return true;
}

bool ChatUser::HandleEnterRoomRequest(const EnterRoomRequest& message) {
  ChatRoomPtr room = server_->FindRoom(message.room_id());
  EnterRoomResponse response;
  do
  {
    if (!room) {
      response.set_error_code(EROOMNAMENOTEXIST);
      break;
    }
    if (room->is_user_exist(user_id_)) {
      response.set_error_code(EALREADYINTHISROOM);
      break;
    }
    if (room->user_count() == room->max_user_count() ) {
      response.set_error_code(EROOMFULL);
      break;
    }
    if (room->room_passwd() != message.room_passwd()) {
      response.set_error_code(EROOMPASSWDERR);
      break;
    }
    room->HandleEnterRoomRequest(shared_from_this());
    return true;
  } while (false);
  Write(MSG_ENTER_ROOM, response);
  return true;
}

bool ChatUser::HandleLeaveRoomRequest(const LeaveRoomRequest& message) {
  ChatRoomPtr room = server_->FindRoom(message.room_id());
  if (room != nullptr) {
    room->HandleLeaveRoomRequest(shared_from_this());
  } else {
    CS_LOG_ERROR("ChatUser::HandleLeaveRoomRequest: room not found");
  }
  return true;
}

bool ChatUser::HandleRoomChatRequest(const RoomChatRequest& message) {
  ChatRoomPtr room = server_->FindRoom(message.room_id());
  if (room != nullptr) {
    room->HandleRoomChatRequest(shared_from_this(), message);
  } else {
    CS_LOG_ERROR("ChatUser::HandleRoomChatRequest: room not found");
  }
  return true;
}

bool ChatUser::HandlePing(const Ping& message) {
  Write(MSG_PING, message);
  return true;
}

bool ChatUser::HandleKeepAliveRequest(const KeepAliveRequest& message) {
  KeepAliveResponse response;
  Write(MSG_KEEP_ALIVE, response);
  return true;
}
