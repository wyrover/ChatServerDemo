#ifndef CHATSERVERCONFIG_H
#define CHATSERVERCONFIG_H

#include <cstdint>
#include <json/json.h>
#include <fstream>
#include <chat_server/Logger.h>

class ChatServerConfig {
public:
  ChatServerConfig() {
    Json::Reader reader;
    Json::Value root;
    std::ifstream fin("./ChatServerConfig.json");
    if (!fin) {
      CS_LOG_FATAL("config file load failed");
    }
    if (!reader.parse(fin, root)) {
      CS_LOG_FATAL("config file parse failed");
    }
    listen_port_ = root["listen_port"].asInt();
  }

  uint16_t listen_port() const {
    return listen_port_;
  }

private:
  uint16_t listen_port_;
};

#endif /* CHATSERVERCONFIG_H */
