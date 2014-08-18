#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "ChatClient.h"

_INITIALIZE_EASYLOGGINGPP

using boost::asio::ip::tcp;

typedef std::shared_ptr<ChatClient> ChatClientPtr;

void login(ChatClientPtr client) {
  std::string login_name, login_passwd;
  std::cout << "input login name:";
  std::cin >> login_name;
  std::cout << "input login password:";
  std::cin >> login_passwd;
  client->Login(login_name, login_passwd);
}

void logout(ChatClientPtr client) {
  std::cout << "log out..." << std::endl;
  client->Logout();
}

void help() {
  std::cout << "available command:" << std::endl;
  std::cout << "login" << std::endl;
  std::cout << "logout" << std::endl;
}

int main(int argc, char* argv[]) {
  boost::asio::io_service ios;
  tcp::resolver resolver(ios);
  auto endpoint_iterator = resolver.resolve({ "127.0.0.1", "30000" });
  ChatClientPtr client = ChatClient::Create(ios, endpoint_iterator);
  std::thread loop([&ios]() {
    ios.run();
  });
  bool exit = false;
  while (!exit) {
    std::string cmd;
    std::cin >> cmd;
    if (cmd == "quit") {
      exit = true;
    } else if (cmd == "login") {
      login(client);
    } else if (cmd == "logout") {
      logout(client);
    } else {
      help();
    }
  }
  ios.stop();
  loop.join();
  return 0;
}
