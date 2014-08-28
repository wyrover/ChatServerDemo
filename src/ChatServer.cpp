#include <chat_server/ChatServer.h>
#include <chat_server/ChatRoom.h>
#include <chat_server/ChatUser.h>
#include <chat_server/wapis.h>
#include <numeric>
#include <chrono>

ChatServer::ChatServer(boost::asio::io_service& ios)
  : ios_(ios), acceptor_(ios), socket_(ios),
    robin_id_(1, std::numeric_limits<uint32_t>::max()),
    timer_(ios) {
}

ChatServer::~ChatServer() {}

ChatRoomPtr ChatServer::CreatRoom(user_id_t creator,
                                  const std::string &room_name,
                                  const std::string& room_passwd,
                                  uint32_t max_user_count) {
  if (room_count() == max_room_count()) {
    CS_LOG_ERROR("resource limit, can not create more rooms");
    return nullptr;
  }
  room_id_t room_id;
  while (true) {
    room_id = static_cast<room_id_t>(robin_id_.next_id());
    if (rooms_.find(room_id) == rooms_.end()) {
      break;
    }
  }
  ChatRoomPtr room = ChatRoom::create(creator, room_id,
                                      room_name, room_passwd, max_user_count, 
                                      shared_from_this());
  rooms_.insert(std::make_pair(room_id, room));
  return room;
}

ChatRoomPtr ChatServer::FindRoom(const room_id_t& id) {
  for (auto it : rooms_) {
    if (it.second->room_id() == id) {
      return it.second;
    }
  }
  return nullptr;
}

ChatUserPtr ChatServer::FindUser(const user_id_t& id) {
  for (ChatUserPtr user : users_) {
    if (user->user_id() == id) {
      return user;
    }
  }
  return nullptr;
}

bool ChatServer::HandleMessage(SessionPtr session,
                               uint16_t message_type,
                               const void *body,
                               int16_t size) {
  do
  {
    if (MSG_USER_LOGIN != message_type) {
      CS_LOG_ERROR("The user is not logged in");
      break;
    }
    UserLoginRequest message;
    if (!message.ParseFromArray(body, size)) {
      CS_LOG_ERROR("UserLoginRequest message parse failed");
      break;
    }
    return HandleUserLogin(session, message);
  } while (false);
  return false;
}

void ChatServer::OnClose(SessionPtr session, CloseReason) {
  auto iter = uncertified_sessions_.find(session);
  if (iter != uncertified_sessions_.end()) {
    uncertified_sessions_.erase(iter);
  } else {
    CS_LOG_ERROR("session not found in the uncertified sessions set");
  }
}

bool ChatServer::HandleUserLogin(SessionPtr session, UserLoginRequest& message) {
  auto self(shared_from_this());
  const std::string login_name = message.login_name();
  const std::string login_passwd = message.login_passwd();
  task_pool_.PutTask([self, session, login_name, login_passwd]() {
    AccountService account_service;
    LoginResultPtr result = account_service.Login(login_name, login_passwd);
    self->ios_.post([self, session, result]() {
      auto iter = self->uncertified_sessions_.find(session);
      if (iter == self->uncertified_sessions_.end()) {
        CS_LOG_DEBUG("session has closed before web service invoke completed");
      } else {
        if (result) {
          self->SendLoginResponse(session, result->error_code());
          if (ESUCCEED == result->error_code()) {
            ChatUserPtr user = self->FindUser(result->user_id());
            if (user != nullptr) {
              CS_LOG_DEBUG("user login again, user id = " << result->user_id());
              user->session()->Disconnect(CloseReason::UNKNOWN);
              user->set_session(session);
            } else {
              CS_LOG_DEBUG("user login, user id = " << result->user_id());
              user = ChatUser::Create(session, self, self->ios_);
              user->set_user_id(result->user_id());
              self->users_.insert(user);
            }
            self->uncertified_sessions_.erase(iter);
          }
        } else {
          CS_LOG_ERROR("internal server error, web service invoke failed");
          self->SendLoginResponse(session, EINTNLSRVFAULT);
        }
      }
    });
  });
  return true;
}

void ChatServer::SendLoginResponse(SessionPtr session, int32_t error_code) {
  UserLoginResponse response;
  response.set_error_code(error_code);
  session->Write(MSG_USER_LOGIN, response);
}

void ChatServer::OnTimer() {
  auto self(shared_from_this());
  timer_.expires_from_now(boost::posix_time::seconds(5));
  timer_.async_wait([self](const error_code &ec) {
    self->OnTimer();
  });
  auto now = std::chrono::steady_clock::now();
  {
    auto iter = uncertified_sessions_.begin();
    while (iter != uncertified_sessions_.end()) {
      SessionPtr session = *iter++;
      auto elapsed = now - session->last_heart_beat();
      auto diff = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
      if (diff > conf_.heart_beat_seconds()) {
        CS_LOG_DEBUG("no response, disconnect");
        session->Disconnect(CloseReason::NORESPONSE);
      }
    }
  }
  {
    auto iter = users_.begin();
    while (iter != users_.end()) {
      SessionPtr session = (*iter++)->session();
      if (session != nullptr) {
        auto elapsed = now - session->last_heart_beat();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        if (diff > conf_.heart_beat_seconds()) {
          CS_LOG_DEBUG("no response, disconnect");
          session->Disconnect(CloseReason::NORESPONSE);
        }
      }
    }
  }
}
