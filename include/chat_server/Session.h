#ifndef CHAT_SERVER_SESSION_H
#define CHAT_SERVER_SESSION_H

#include <memory>
#include <queue>
#include <boost/asio.hpp>
#include <avxer/Buffer.h>
#include "Macros.h"
#include "ChatServerPacket.h"

using boost::asio::ip::tcp;
using boost::asio::io_service;
using avxer::Buffer;

class Session : public std::enable_shared_from_this<Session> {
public:
  typedef std::function<bool(SessionPtr, uint16_t, const void*, int16_t)> on_message_cb_t;
  typedef std::function<void(SessionPtr)> on_close_cb_t;

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

  void set_on_message_cb(const on_message_cb_t& cb) {
    auto self(shared_from_this());
    ios_.dispatch([self, cb] () {
      self->on_message_cb_ = cb;
    });
  }

  void set_on_close_cb(const on_close_cb_t& cb) {
    auto self(shared_from_this());
    ios_.dispatch([self, cb]() {
      self->on_close_cb_ = cb;
    });
  }

  /**
   * Close the connection
   * @param signal Whether invoke on_close_cb_ or not
   * @return 
   */
  void Disconnect(bool signal) {
    auto self(shared_from_this());
    ios_.post([self, signal]() {
      if (!self->socket_.is_open()) {
        return;
      }
      self->socket_.close();
      if (signal && self->on_close_cb_) {
        self->on_close_cb_(self);
      }
    });
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
};

#endif /* CHAT_SERVER_SESSION_H */
