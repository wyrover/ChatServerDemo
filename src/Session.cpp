#include <chat_server/Session.h>
#include <chat_server/Logger.h>

using namespace boost::asio;

Session::Session(io_service &ios, tcp::socket &&socket)
  : ios_(ios), socket_(std::move(socket)), header_data_(PACKET_HEADER_LENGTH) {
}

bool Session::Initial() {
  AsyncReadHeader();
  return true;
}

Session::~Session() {
}

void Session::AsyncReadHeader() {
  auto self(shared_from_this());
  async_read(socket_, buffer(&header_data_[0], PACKET_HEADER_LENGTH),
    [this, self](boost::system::error_code ec, std::size_t size) {
    if (!ec) {
      AsyncReadBody();
    } else {
      CS_LOG_ERROR("async_read error: " << ec.message());
      self->Disconnect(true);
    }
  });
}

void Session::AsyncReadBody() {
  PacketHeader header;
  header.Deserialize(header_data_);
  const uint16_t body_length = header.length() - PACKET_HEADER_LENGTH;
  if (body_length) {
    body_data_.reset(new char[body_length]);
    auto self(shared_from_this());
    async_read(socket_, buffer(&body_data_[0], body_length),
      [this, self](boost::system::error_code ec, std::size_t size) {
      if (!ec) {
        PacketHeader header;
        header.Deserialize(header_data_);
        if (HandleMessage(header.message_type(), body_data_.get(),
                          header.length() - PACKET_HEADER_LENGTH)) {
          AsyncReadHeader();
        }
      } else {
        CS_LOG_ERROR("async_read error: " << ec.message());
        self->Disconnect(true);
      }
    });
  } else {
    body_data_.reset();
    if (HandleMessage(header.message_type(), nullptr, 0)) {
      AsyncReadHeader();
    }
  }
}

void Session::Write(uint16_t type, const MessageLite &message) {
  BufferPtr buffer = SerializeMessage(message, type);
  auto self(shared_from_this());
  ios_.post([self, buffer] () {
    bool is_empty = self->write_queue_.empty();
    self->write_queue_.push(buffer);
    if (is_empty) {
      self->AsyncWrite();
    }
  });
}

void Session::AsyncWrite() {
  auto self(shared_from_this());
  BufferPtr buf = write_queue_.front();
  async_write(socket_, buffer(&(*buf)[0], buf->length()),
    [self, this](boost::system::error_code ec, std::size_t size) {
    if (!ec) {
      write_queue_.pop();
      if (!write_queue_.empty()) {
        AsyncWrite();
      }
    } else {
      CS_LOG_ERROR("async_write error: " << ec.message());
    }
  });
}

bool Session::HandleMessage(uint16_t type, const void *body, int16_t size) {
  bool handled = false;
  if (on_message_cb_) {
    handled = on_message_cb_(shared_from_this(), type, body, size);
  }
  if (!handled) {
    Disconnect(true);
  }
  return handled;
}
