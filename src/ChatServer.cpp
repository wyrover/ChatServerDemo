#include <chat_server/ChatServer.h>

ChatServer::ChatServer(boost::asio::io_service& ios)
  : ios_(ios), acceptor_(ios), socket_(ios) {
}

ChatServer::~ChatServer() {}
