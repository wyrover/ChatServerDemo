#ifndef CHAT_SERVER_WAPIS_H
#define CHAT_SERVER_WAPIS_H

#include <string>
#include <memory>
#include "WapisInvokeResult.h"

class AccountService {
public:
  static LoginResultPtr Login(const std::string& login_name,
                              const std::string& login_passwd);
};

#endif /* CHAT_SERVER_WAPIS_H */
