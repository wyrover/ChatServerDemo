#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include "ChatClient.h"

#ifdef CHAT_SERVER_LOGGER
_INITIALIZE_EASYLOGGINGPP
#endif

using boost::asio::ip::tcp;

typedef std::shared_ptr<ChatClient> ChatClientPtr;

void login(ChatClientPtr client) {
  std::string login_name, login_passwd;
  std::cout << "input login_name login_passwd:";
  std::cin >> login_name >> login_passwd;
  client->Login(login_name, login_passwd);
}

void logout(ChatClientPtr client) {
  client->Logout();
}

void creat_room(ChatClientPtr client) {
  std::string room_name, room_passwd;
  uint32_t max_usr_count;
  std::cout << "input room_name room_passwd max_usr_count:";
  std::cin >> room_name >> room_passwd >> max_usr_count;
  client->CreatRoom(room_name, room_passwd, max_usr_count);
}

void enter_room(ChatClientPtr client) {
  room_id_t room_id;
  std::string room_passwd;
  std::cout << "input room_id room_passwd:";
  std::cin >> room_id >> room_passwd;
  client->EnterRoom(room_id, room_passwd);
}

void room_message(ChatClientPtr client) {
  room_id_t room_id;
  std::string message;
  std::cout << "input the room_id message:";
  std::cin >> room_id >> message;
  client->RoomMessage(room_id, message.c_str(), message.size() + 1);
}

void leave_room(ChatClientPtr client) {
  room_id_t room_id;
  std::cout << "input the room_id:";
  std::cin >> room_id;
  client->LeaveRoom(room_id);
}

void help() {
  std::cout << "available command:" << std::endl;
  std::cout << "login" << std::endl;
  std::cout << "logout" << std::endl;
}

int main(int argc, char* argv[]) {
#ifdef CHAT_SERVER_LOGGER
  el::Loggers::reconfigureAllLoggers(el::Configurations("./easylogging_client.conf"));
#endif

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
    } else if (cmd == "creat_room") {
      creat_room(client);
    } else if (cmd == "enter_room") {
      enter_room(client);
    } else if (cmd == "room_message") {
      room_message(client);
    } else if (cmd == "leave_room") {
      leave_room(client);
    } else {
      help();
    }
  }
  ios.stop();
  loop.join();
  return 0;
}
