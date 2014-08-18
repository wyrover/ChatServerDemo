#include <chat_server/Logger.h>
#include <chat_server/ChatServer.h>
#include <chat_server/ChatServerConfig.h>
#include <boost/asio.hpp>
#include <thread>
#include <vld.h>

using boost::asio::ip::tcp;

#define COMMAND_QUIT "quit"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
  el::Loggers::reconfigureAllLoggers(el::Configurations("./easylogging.conf"));

  ChatServerConfig conf;
  tcp::endpoint endpoint(tcp::v4(), conf.listen_port());
  boost::asio::io_service ios;
  ChatServerPtr server = ChatServer::create(ios, endpoint);
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
    if (cmd == COMMAND_QUIT) {
      std::cout << "Are sure to stop server?(y/n):";
      char choose;
      std::cin >> choose;
      exit = choose == 'y';
    } else {
      std::cout << "Unknown command!" << std::endl;
    }
  }
  ios.stop();
  loop.join();
  google::protobuf::ShutdownProtobufLibrary();
  CS_LOG_DEBUG("chat server has been stopped");
  return 0;
}
