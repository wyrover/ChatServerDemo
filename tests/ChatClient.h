#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <memory>
#include <cstdio>
#include <boost/asio.hpp>
#include <chat_server/Logger.h>
#include <chat_server/Session.h>
#include <chat_server/ChatServerPacket.h>
#include <chat_server/ChatServerErrorCode.h>

using boost::asio::ip::tcp;
using namespace com::avxer::chat;

class ChatClient : public std::enable_shared_from_this<ChatClient> {
public:
  typedef std::shared_ptr<ChatClient> ChatClientPtr;
  static ChatClientPtr Create(boost::asio::io_service& ios,
                              tcp::resolver::iterator endpoint_iterator) {
    ChatClientPtr client = std::make_shared<ChatClient>(ios);
    if (client && client->Initial(endpoint_iterator)) {
      return client;
    }
    return nullptr;
  }

  ChatClient(boost::asio::io_service& ios) : ios_(ios) {
  }

  ~ChatClient() {}

  void Login(const std::string &login_name, const std::string &login_passwd);
  void Logout();
  void CreatRoom(const std::string &room_name, const std::string& room_passwd, uint32_t max_user_count);
  void EnterRoom(const room_id_t& room_id, const std::string& room_passwd);
  void SendGroupMessage(const room_id_t& room_id, const void* data, uint32_t size);

private:
  bool Initial(tcp::resolver::iterator endpoint_iterator) {
    auto self(shared_from_this());
    std::shared_ptr<tcp::socket> socket(new tcp::socket(ios_));
    boost::asio::async_connect(*socket, endpoint_iterator,
      [this, self, socket](boost::system::error_code ec, tcp::resolver::iterator) {
      if (!ec) {
        CS_LOG_ERROR("connect to server ok.");
        session_ = Session::Create(self->ios_, std::move(*socket));
        session_->set_on_message_cb(std::bind(&ChatClient::HandleMessage, self,
          std::placeholders::_1, std::placeholders::_2,
          std::placeholders::_3, std::placeholders::_4));
        session_->set_on_close_cb(std::bind(&ChatClient::OnClose, self, std::placeholders::_1));
      } else {
        CS_LOG_ERROR("connect to server failed!");
      }
    });
    return true;
  }

  bool HandleMessage(SessionPtr session, uint16_t type, const void* body, int16_t size);
  void OnClose(SessionPtr session);

  bool HandleUserLoginResponse(const UserLoginResponse& response);
  bool HandleCreatRoomResponse(const CreatRoomResponse& message);
  bool HandleEnterRoomResponse(const EnterRoomResponse& message);
  bool HandleUserEntered(const UserEnteredNtfy& message);
  bool HandleUserLeaved(const UserLeavedNtfy& message);
  bool HandleGroupMessage(const GroupChatNtfy& message);

private:
  boost::asio::io_service &ios_;
  SessionPtr session_;
};

#endif /* CHATCLIENT_H */
