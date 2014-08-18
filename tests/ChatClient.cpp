#include "ChatClient.h"

using namespace boost::asio;

void ChatClient::Login(const std::string &login_name,
                       const std::string &login_passwd) {
  UserLoginRequest login_msg;
  login_msg.set_login_name(login_name);
  login_msg.set_login_passwd(login_passwd);
  if (session_) {
    session_->Write(MSG_USER_LOGIN_REQUEST, login_msg);
  }
}

void ChatClient::Logout() {
  UserLogoutRequest logout_msg;
  BufferPtr buffer = SerializeMessage(logout_msg, MSG_USER_LOGOUT);
  if (session_) {
    session_->Write(MSG_USER_LOGOUT, logout_msg);
  }
}

bool ChatClient::HandleMessage(SessionPtr session, uint16_t type, const void* body, int16_t size) {
  bool handled = false;
  bool parse_ok = false;
  if (MSG_USER_LOGIN_RESPONSE == type) {
    UserLoginResponse response;
    if (parse_ok = response.ParseFromArray(body, size)) {
      handled = HandleUserLoginResponse(response);
    }
  } else {
    CS_LOG_ERROR("Unknown message type = " << type);
    return false;
  }
  if (!parse_ok) {
    CS_LOG_ERROR("parse failed, type = " << type);
  }
  return handled;
}

void ChatClient::OnClose(SessionPtr session) {
  CS_LOG_ERROR("ChatClient::OnClose");
}

bool ChatClient::HandleUserLoginResponse(const UserLoginResponse& response) {
  CS_LOG_ERROR("HandleUserLoginResponse: error_code = " << response.error_code());
  return true;
}
