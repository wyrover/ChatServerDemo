#ifndef CHAT_SERVER_WAPISINVOKERESULT_H
#define CHAT_SERVER_WAPISINVOKERESULT_H

#include <json/json.h>
#include <memory>
#include <string>
#include "ChatServerErrorCode.h"

class WapisInvokeResult {
public:
  WapisInvokeResult();
  virtual ~WapisInvokeResult() {}

  int error_code() const {
    return error_code_;
  }

  bool Parse(const char* result, size_t size);

protected:
  virtual bool DoParse(Json::Value& root) {
    return true;
  }

protected:
  int error_code_;
};


class LoginResult : public WapisInvokeResult {
public:
  bool DoParse(Json::Value& root) override {
    if (error_code_ == ESUCCEED) {
      user_id_ = root["user_id"].asString();
    }
    return true;
  }

  std::string user_id() const { return user_id_; }

private:
  std::string user_id_;
};

typedef std::shared_ptr<LoginResult> LoginResultPtr;

#endif /* CHAT_SERVER_WAPISINVOKERESULT_H */
