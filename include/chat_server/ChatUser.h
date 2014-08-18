#ifndef CHAT_SERVER_USER_H
#define CHAT_SERVER_USER_H

#include "Macros.h"
#include "ChatServerPacket.h"
#include <memory>

using namespace avxer::chat;

class ChatUser : public std::enable_shared_from_this<ChatUser> {
public:
  static ChatUserPtr Create(SessionPtr session, ChatServerPtr server) {
    ChatUserPtr user = std::make_shared<ChatUser>(session, server);
    if (user && user->Initial()) {
      return user;
    }
    return nullptr;
  }

  ChatUser(SessionPtr session, ChatServerPtr server);
  ~ChatUser();

private:
  bool Initial();

  bool HandleMessage(SessionPtr session,
                     uint16_t message_type,
                     const void *body,
                     int16_t size);
  void OnClose(SessionPtr session);

  bool HandleUserLogin(SessionPtr, UserLoginRequest *message);
  bool HandleUserLogout(SessionPtr, UserLogoutRequest *message);

private:
  SessionRole role_;
  SessionPtr session_;
  ChatServerPtr server_;
};

#endif /* CHAT_SERVER_USER_H */
