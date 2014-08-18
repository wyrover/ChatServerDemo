#ifndef CHAT_SERVER_WAPISINVOKERESULT_H
#define CHAT_SERVER_WAPISINVOKERESULT_H

#include <memory>
#include <json/json.h>

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
};

typedef std::shared_ptr<LoginResult> LoginResultPtr;

#endif /* CHAT_SERVER_WAPISINVOKERESULT_H */
