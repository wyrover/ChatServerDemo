#include <chat_server/ChatUser.h>
#include <chat_server/Logger.h>
#include <chat_server/wapis.h>
#include <chat_server/ChatServerErrorCode.h>
#include <chat_server/Session.h>
#include <chat_server/ChatServer.h>
#include <cassert>

ChatUser::ChatUser(SessionPtr session, ChatServerPtr server)
  : role_(SessionRole::Uncertified), session_(session), server_(server) {
}

ChatUser::~ChatUser() {
}

bool ChatUser::Initial() {
  auto self(shared_from_this());
  session_->set_on_message_cb(std::bind(&ChatUser::HandleMessage, self,
    std::placeholders::_1, std::placeholders::_2,
    std::placeholders::_3, std::placeholders::_4));
  session_->set_on_close_cb(std::bind(&ChatUser::OnClose, self,
    std::placeholders::_1));
  return true;
}

bool ChatUser::HandleMessage(SessionPtr session,
                             uint16_t message_type,
                             const void *body,
                             int16_t size) {
  if (MSG_USER_LOGIN_REQUEST == message_type) {
    UserLoginRequest message;
    message.ParseFromArray(body, size);
    return HandleUserLogin(session, &message);
  } else if (MSG_USER_LOGOUT == message_type) {
    UserLogoutRequest message;
    message.ParseFromArray(body, size);
    return HandleUserLogout(session, &message);
  } else {
    return false;
  }
}

void ChatUser::OnClose(SessionPtr session) {
  CS_LOG_DEBUG(__FUNCTION__);
  server_->remove_user(shared_from_this());
}

bool ChatUser::HandleUserLogin(SessionPtr, UserLoginRequest *message) {
  const std::string login_name = message->login_name();
  const std::string login_passwd = message->login_passwd();
  CS_LOG_DEBUG(__FUNCTION__ << ": " << login_name << ", " << login_passwd);
  auto self(shared_from_this());
  server_->task_pool().PutTask([self, login_name, login_passwd]() {
    LoginResultPtr result = AccountService::Login(login_name, login_passwd);
    self->server_->ios().post([self, result]() {
      if (!self->server_->is_user_exist(self)) {
        CS_LOG_DEBUG("session has closed before web service invoke completed");
        return;
      }
      UserLoginResponse response;
      if (result) {
        response.set_error_code(result->error_code());
      } else {
        CS_LOG_ERROR("Internal server error");
        response.set_error_code(EINTNLSRVCOMMU);
      }
      self->session_->Write(MSG_USER_LOGIN_RESPONSE, response);
    });
  });
  return true;
}

bool ChatUser::HandleUserLogout(SessionPtr, UserLogoutRequest *message) {
  CS_LOG_DEBUG(__FUNCTION__);
  return false;
}
