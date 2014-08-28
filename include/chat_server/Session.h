#ifndef CHAT_SERVER_SESSION_H
#define CHAT_SERVER_SESSION_H

#include <memory>
#include <queue>
#include <chrono>
#include <boost/asio.hpp>
#include <avxer/Buffer.h>
#include "Macros.h"
#include "ChatServerPacket.h"

using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::system::error_code;
using avxer::Buffer;

enum class CloseReason {
  UNKNOWN,
  READEOF,            // 对方关闭了链接
  NORESPONSE,         // 超时未收到对方的数据
  MESSAGEUNHANDLED    // 消息未被处理
};

class Session : public std::enable_shared_from_this<Session> {
public:
  typedef std::function<bool(SessionPtr, uint16_t, const void*, int16_t)> on_message_cb_t;
  typedef std::function<void(SessionPtr, CloseReason)> on_close_cb_t;

  static SessionPtr Create(io_service &ios, tcp::socket &&socket) {
    SessionPtr session = std::make_shared<Session>(ios, std::move(socket));
    if (session && session->Initial()) {
      return session;
    }
    return nullptr;
  }

  Session(io_service &ios, tcp::socket &&socket);
  ~Session();

  void Write(uint16_t type, const google::protobuf::MessageLite &message);

  void set_on_message_cb(const on_message_cb_t& cb = on_message_cb_t()) {
    auto self(shared_from_this());
    ios_.dispatch([self, cb] () {
      self->on_message_cb_ = cb;
    });
  }

  void set_on_close_cb(const on_close_cb_t& cb = on_close_cb_t()) {
    auto self(shared_from_this());
    ios_.dispatch([self, cb]() {
      self->on_close_cb_ = cb;
    });
  }

  /**
   * Close the connection
   * @param reason close callback won't be called if reason is UNKNOWN
   */
  void Disconnect(CloseReason reason) {
    auto self(shared_from_this());
    ios_.post([self, reason]() {
      if (!self->socket_.is_open()) {
        return;
      }
      error_code ec;
      self->socket_.shutdown(tcp::socket::shutdown_receive, ec);
      if (ec) {
        CS_LOG_ERROR("socket shutdown failed: " << ec.message());
      }
      self->socket_.close(ec);
      if (ec) {
        CS_LOG_ERROR("socket close failed: " << ec.message());
      }
      if (CloseReason::UNKNOWN != reason && self->on_close_cb_) {
        self->on_close_cb_(self, reason);
      }
    });
  }

  std::chrono::steady_clock::time_point last_heart_beat() const {
    return last_heart_beat_;
  }

private:
  bool Initial();
  void AsyncReadHeader();
  void AsyncReadBody();
  bool HandleMessage(uint16_t type, const void *body, int16_t size);
  void AsyncWrite();

private:
  boost::asio::io_service &ios_;
  tcp::socket socket_;
  Buffer header_data_;
  std::unique_ptr<char[]> body_data_;
  std::queue<BufferPtr> write_queue_;
  on_message_cb_t on_message_cb_;
  on_close_cb_t on_close_cb_;
  std::chrono::steady_clock::time_point last_heart_beat_;
};

#endif /* CHAT_SERVER_SESSION_H */
