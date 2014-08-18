#include <chat_server/HttpClient.h>
#include <thread>
#include <cassert>
#include <iostream>

class __curl_raii {
public:
  typedef size_t(*write_cb_t)(void*, size_t, size_t, void*);

  __curl_raii() {
    curl_ = nullptr;
    http_header_ = nullptr;
  }

  bool initial(const char *url,
               write_cb_t write_function,
               void *write_stream,
               write_cb_t header_function,
               void *header_stream,
               const std::vector<std::string>& headers) {
    for (const std::string& header : headers) {
      http_header_ = curl_slist_append(http_header_, header.c_str());
    }
    curl_ = curl_easy_init();
    return curl_ != nullptr && setopt(CURLOPT_CONNECTTIMEOUT, 30)
      && setopt(CURLOPT_TIMEOUT, 60)
      && setopt(CURLOPT_SSL_VERIFYPEER, 0L)
      && setopt(CURLOPT_SSL_VERIFYHOST, 0L)
      && setopt(CURLOPT_NOSIGNAL, 1L)
      && setopt(CURLOPT_URL, url)
      && setopt(CURLOPT_WRITEFUNCTION, write_function)
      && setopt(CURLOPT_WRITEDATA, write_stream)
      && setopt(CURLOPT_HEADERFUNCTION, header_function)
      && setopt(CURLOPT_HEADERDATA, header_stream)
      && setopt(CURLOPT_HTTPHEADER, http_header_);
  }

  ~__curl_raii() {
    if (curl_ != nullptr) {
      curl_easy_cleanup(curl_);
    }
    if (http_header_ != nullptr) {
      curl_slist_free_all(http_header_);
    }
  }

  template<typename T>
  bool setopt(CURLoption option, const T &value) {
    return CURLE_OK == curl_easy_setopt(curl_, option, value);
  }

  bool perform() {
    return CURLE_OK == curl_easy_perform(curl_);
  }

  bool GetResponseCode(long &response_code) {
    return CURLE_OK == curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
  }

private:
  CURL *curl_;
  curl_slist *http_header_;
};

HttpClient::HttpClient() {
}

HttpClient::~HttpClient() {
}

HttpResponsePtr HttpClient::Execute(const HttpPost &request) {
  __curl_raii curl;
  const std::string url = request.url();
  const std::string parameter = request.parameter();
  HttpResponsePtr response(new HttpResponse());
  bool result = curl.initial(url.c_str(),
    &HttpClient::WriteBody,
    &response->body_data(),
    &HttpClient::WriteHeader,
    &response->header_data(),
    request.headers())
    && curl.setopt(CURLOPT_POST, 1)
    && curl.setopt(CURLOPT_POSTFIELDS, parameter.c_str())
    && curl.setopt(CURLOPT_POSTFIELDSIZE, parameter.size());
  if (!result) {
    return nullptr;
  }
  if (!curl.perform()) {
    return nullptr;
  }
  long response_code = 0;
  if (!curl.GetResponseCode(response_code)) {
    return nullptr;
  }
  response->set_response_code(response_code);
  return response;
}

size_t HttpClient::WriteBody(void *buffer, size_t sz, size_t nmemb, void *userdata) {
  std::vector<char> *vec = (std::vector<char>*)userdata;
  size_t size = sz * nmemb;
  vec->insert(vec->end(), (char*)buffer, (char*)buffer + size);
  return size;
}

size_t HttpClient::WriteHeader(void *buffer, size_t sz, size_t nmemb, void *userdata) {
  std::vector<char> *vec = (std::vector<char>*)userdata;
  size_t size = sz * nmemb;
  vec->insert(vec->end(), (char*)buffer, (char*)buffer + size);
  return size;
}
