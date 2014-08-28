#ifndef CHATSERVERCONFIG_H
#define CHATSERVERCONFIG_H

#include <cstdint>
#include <json/json.h>
#include <fstream>
#include <string>
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
    heart_beat_seconds_ = root["heart_beat_seconds"].asInt();
    offline_wait_seconds_ = root["offline_wait_seconds"].asInt();
    web_service_url_ = root["web_service_url"].asString();
  }

  uint16_t listen_port() const { return listen_port_; }
  uint32_t max_room_count() const { return max_room_count_; }
  uint32_t heart_beat_seconds() const { return heart_beat_seconds_; }
  uint32_t offline_wait_seconds() const { return offline_wait_seconds_; }
  std::string web_service_url() const { return web_service_url_; }

private:
  uint16_t listen_port_;
  uint32_t max_room_count_;
  uint32_t heart_beat_seconds_;
  uint32_t offline_wait_seconds_;
  std::string web_service_url_;
};

#endif /* CHATSERVERCONFIG_H */
