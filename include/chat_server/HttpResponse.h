#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <vector>
#include <memory>

class HttpResponse {
public:
  HttpResponse() {}
  ~HttpResponse() {}

  void set_response_code(long response_code) {
    response_code_ = response_code;
  }

  int response_code() const {
    return response_code_;
  }

  std::vector<char>& header_data() {
    return header_data_;
  }

  std::vector<char>& body_data() {
    return body_data_;
  }

private:
  long response_code_;
  std::vector<char> header_data_;
  std::vector<char> body_data_;
};

typedef std::shared_ptr<HttpResponse> HttpResponsePtr;

#endif /* HTTPRESPONSE_H */
