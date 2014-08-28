#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <chat_server/Logger.h>
#include <chat_server/ChatServer.h>
#ifdef _MSC_VER
#include <vld.h>
#endif

#ifdef CHAT_SERVER_LOGGER
_INITIALIZE_EASYLOGGINGPP
#endif

int main(int argc, char* argv[]) {
#ifdef CHAT_SERVER_LOGGER
  el::Loggers::reconfigureAllLoggers(el::Configurations("./easylogging.conf"));
#endif
  boost::asio::io_service ios;
  ChatServerPtr server = ChatServer::create(ios);
  if (!server) {
    CS_LOG_ERROR("chat server init failed");
    return -1;
  }
  CS_LOG_DEBUG("chat server has been started");
  std::thread loop([&ios]() { ios.run(); });

  bool exit = false;
  while (!exit) {
    std::string cmd;
    std::cin >> cmd;
    if (cmd == "quit") {
      std::cout << "Are sure to stop server?(y/n):";
      char choose;
      std::cin >> choose;
      exit = choose == 'y';
    } else {
      std::cout << "Unknown command!" << std::endl;
    }
  }
  server->stop();
  ios.stop();
  loop.join();
  google::protobuf::ShutdownProtobufLibrary();
  CS_LOG_DEBUG("chat server has been stopped");
  return 0;
}
