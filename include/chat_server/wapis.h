#ifndef CHAT_SERVER_WAPIS_H
#define CHAT_SERVER_WAPIS_H

#include <string>
#include <memory>
#include "WapisInvokeResult.h"
#include "ChatServerConfig.h"

class AccountService {
public:
  LoginResultPtr Login(const std::string& login_name,
                       const std::string& login_passwd);

private:
  ChatServerConfig conf_;
};

#endif /* CHAT_SERVER_WAPIS_H */
