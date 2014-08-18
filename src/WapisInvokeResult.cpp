#include <chat_server/WapisInvokeResult.h>
#include <chat_server/Logger.h>
#include <chat_server/ChatServerErrorCode.h>

WapisInvokeResult::WapisInvokeResult() {
  error_code_ = EFAILED;
}

bool WapisInvokeResult::Parse(const char* result, size_t size) {
  Json::Value root;
  Json::Reader reader;
  std::string document(result, result + size);
  if (!reader.parse(document, root)) {
    CS_LOG_ERROR("JSON document parse failed: " << document);
    return false;
  }
  error_code_ = root["error_code"].asInt();
  return DoParse(root);
}
