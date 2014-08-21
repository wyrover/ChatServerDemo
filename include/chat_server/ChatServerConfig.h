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
    max_room_count_ = root["max_room_count"].asInt();
  }

  uint16_t listen_port() const { return listen_port_; }

  uint32_t max_room_count() const { return max_room_count_; }

private:
  uint16_t listen_port_;
  uint32_t max_room_count_;
};

#endif /* CHATSERVERCONFIG_H */
