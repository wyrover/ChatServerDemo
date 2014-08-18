#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <memory>
#include <functional>
#include <vector>

class HttpPost {
public:
  HttpPost(const std::string &url) : url_(url) {
  }

  void SetEntity(const std::string &parameter) {
    parameter_ = parameter;
  }

  const std::string& url() const {
    return url_;
  }

  const std::string& parameter() const {
    return parameter_;
  }

  const std::vector<std::string>& headers() const {
    return headers_;
  }

  void AddHeader(const std::string& header) {
    headers_.push_back(header);
  }

private:
  std::string url_;
  std::string parameter_;
  std::vector<std::string> headers_;
};

#endif /* HTTPREQUEST_H */
