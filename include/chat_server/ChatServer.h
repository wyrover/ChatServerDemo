#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <memory>
#include <set>
#include <cstdint>
#include <boost/asio.hpp>
#include <chat_server/Session.h>
#include <chat_server/Logger.h>
#include <chat_server/Macros.h>
#include <chat_server/TaskPool.h>
#include <chat_server/ChatUser.h>

using boost::asio::ip::tcp;
using boost::system::error_code;

class ChatServer : public std::enable_shared_from_this<ChatServer> {
public:
  static ChatServerPtr create(boost::asio::io_service& ios,
                              const tcp::endpoint& endpoint) {
    ChatServerPtr server = std::make_shared<ChatServer>(ios);
    if (server && server->initial(endpoint)) {
      return server;
    }
    return nullptr;
  }

  ChatServer(boost::asio::io_service& ios);
  ~ChatServer();

  bool remove_user(ChatUserPtr user) {
    auto iter = users_.find(user);
    if (iter != users_.end()) {
      users_.erase(iter);
      CS_LOG_ERROR("remove a user, still have [" << users_.size() << "] users");
      return true;
    } else {
      CS_LOG_ERROR("RemoveUser: user not found");
      return false;
    }
  }

  bool is_user_exist(ChatUserPtr user) const {
    return users_.find(user) != users_.end();
  }

  boost::asio::io_service& ios() {
    return ios_;
  }

  TaskPool& task_pool() {
    return task_pool_;
  }

private:
  bool initial(const tcp::endpoint& endpoint) {
    do 
    {
      boost::system::error_code ec;
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
        ChatUserPtr user = ChatUser::Create(session, self);
        self->users_.insert(user);
      } else {
        CS_LOG_ERROR("async_accept error: " << error.message());
      }
      self->AsyncAccept();
    });
  }

private:
  boost::asio::io_service& ios_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::set<ChatUserPtr> users_;
  TaskPool task_pool_;
};

#endif /* CHATSERVER_H */
