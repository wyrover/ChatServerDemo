#ifndef CHAT_SERVER_MACROS_H
#define CHAT_SERVER_MACROS_H

#include <memory>

class Session;
class ChatServer;
class ChatUser;

typedef std::shared_ptr<Session> SessionPtr;
typedef std::weak_ptr<Session> SessionWkPtr;
typedef std::shared_ptr<ChatServer> ChatServerPtr;
typedef std::shared_ptr<ChatUser> ChatUserPtr;

enum class SessionRole {
  Uncertified,         /** δ��֤ */
  User,                /** �û� */
  Administrator        /** ����Ա */
};

#define SESSION_MESSAGE_HANDLER_DECLARE()\
  bool HandleMessage(uint16_t message_type, const void *body, int16_t size);

#define SESSION_MESSAGE_HANDLER_BEGIN(__class__)\
  bool __class__::HandleMessage(uint16_t message_type, const void *body, int16_t size) {\
  switch (message_type) {

#define SESSION_MESSAGE_HANDLER(__message_type__, __class__, __func__)\
  case __message_type__:{\
    __class__ message;\
    message.ParseFromArray(body, size);\
    return __func__(message);}

#define SESSION_MESSAGE_HANDLER_END() } return false;};

#endif /* CHAT_SERVER_MACROS_H */
