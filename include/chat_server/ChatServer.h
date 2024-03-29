#ifndef CHAT_SERVER_CHATSERVER_H
#define CHAT_SERVER_CHATSERVER_H

#include <set>
#include <map>
#include <memory>
#include <cstdint>
#include <numeric>
#include <cassert>
#include <boost/asio.hpp>
#include "Session.h"
#include "Logger.h"
#include "Macros.h"
#include "TaskPool.h"
#include "ChatUser.h"
#include "robin_id.h"
#include "ChatServerConfig.h"

using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::asio::deadline_timer;
using boost::system::error_code;

class ChatServer : public std::enable_shared_from_this<ChatServer> {
public:
  static ChatServerPtr create(io_service& ios) {
    ChatServerPtr server = std::make_shared<ChatServer>(ios);
    if (server && server->initial()) {
      return server;
    }
    return nullptr;
  }

  ChatServer(io_service& ios);
  ~ChatServer();

  void stop() {
    auto self(shared_from_this());
    ios_.post([self]() {
      self->uncertified_sessions_.clear();
      self->users_.clear();
      self->rooms_.clear();
    });
  }

  bool is_user_exist(ChatUserPtr user) const {
    return users_.find(user) != users_.end();
  }

  bool remove_user(ChatUserPtr user) {
    auto iter = users_.find(user);
    if (iter != users_.end()) {
      users_.erase(iter);
      CS_LOG_DEBUG("remove a user, still have [" << users_.size() << "] users");
      return true;
    } else {
      CS_LOG_ERROR("remove_user: user not found");
      return false;
    }
  }

  bool remove_room(room_id_t id) {
    auto iter = rooms_.find(id);
    if (iter != rooms_.end()) {
      rooms_.erase(iter);
      CS_LOG_DEBUG("remove a room, still have [" << rooms_.size() << "] rooms");
      return true;
    } else {
      CS_LOG_ERROR("remove_room: room not found");
      return false;
    }
  }

  uint32_t max_room_count() const { return conf_.max_room_count(); }
  uint32_t room_count() const { return static_cast<uint32_t>(rooms_.size()); }

  ChatRoomPtr CreatRoom(user_id_t creator,
                        const std::string &room_name,
                        const std::string& room_passwd,
                        uint32_t max_user_count);

  ChatRoomPtr FindRoom(const room_id_t& id);
  ChatUserPtr FindUser(const user_id_t& id);

  ChatServerConfig& conf() { return conf_; }

private:
  bool initial() {
    do 
    {
      tcp::endpoint endpoint(tcp::v4(), conf_.listen_port());
      error_code ec;
      acceptor_.open(endpoint.protocol(), ec);
      if (ec) {
        CS_LOG_ERROR("open failed: " << ec.message());
        break;
      }
      acceptor_.bind(endpoint, ec);
      if (ec) {
        CS_LOG_ERROR("bind failed: " << ec.message());
        break;
      }
      acceptor_.listen(tcp::acceptor::max_connections, ec);
      if (ec) {
        CS_LOG_ERROR("listen failed: " << ec.message());
        break;
      }
      AsyncAccept();
      OnTimer();
      return true;
    } while (false);
    return false;
  }

  void AsyncAccept() {
    auto self = shared_from_this();
    acceptor_.async_accept(socket_, [this, self](const error_code &error) {
      if (!error) {
#ifdef CHAT_SERVER_DEBUG
        error_code ec;
        socket_.remote_endpoint(ec);
        if (!ec) {
          CS_LOG_DEBUG("accept from " << socket_.remote_endpoint());
        }
#endif
        SessionPtr session = Session::Create(self->ios_, std::move(self->socket_));
        session->set_on_message_cb(std::bind(&ChatServer::HandleMessage, self,
          std::placeholders::_1, std::placeholders::_2,
          std::placeholders::_3, std::placeholders::_4));
        session->set_on_close_cb(std::bind(&ChatServer::OnClose, self, std::placeholders::_1,
          std::placeholders::_2));
        uncertified_sessions_.insert(session);
      } else {
        CS_LOG_ERROR("async_accept error: " << error.message());
      }
      self->AsyncAccept();
    });
  }

  bool HandleMessage(SessionPtr session,
                     uint16_t message_type,
                     const void *body,
                     int16_t size);
  void OnClose(SessionPtr session, CloseReason);
  bool HandleUserLogin(SessionPtr session, UserLoginRequest& message);
  void SendLoginResponse(SessionPtr session, int32_t error_code);

  void OnTimer();

private:
  io_service& ios_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  TaskPool task_pool_;
  deadline_timer timer_;

  std::set<SessionPtr> uncertified_sessions_;

  std::set<ChatUserPtr> users_;
  robin_id robin_id_;
  std::map<room_id_t, ChatRoomPtr> rooms_;

  ChatServerConfig conf_;
};

#endif /* CHAT_SERVER_CHATSERVER_H */
