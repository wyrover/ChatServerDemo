#ifndef CHAT_SERVER_HTTPCLIENT_H
#define CHAT_SERVER_HTTPCLIENT_H

#include <string>
#include <curl/curl.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

#define CONTENT_TYPE_URLENCODED "Content-Type: application/x-www-form-urlencoded"
#define CONTENT_TYPE_JSON       "Content-Type: application/json"

class HttpClient {
public:
  HttpClient();
  ~HttpClient();
  HttpResponsePtr Execute(const HttpPost &post_request);

  HttpClient(const HttpClient&) = delete;
  HttpClient& operator=(const HttpClient&) = delete;

private:
  static size_t WriteBody(void *buffer, size_t sz, size_t nmemb, void *userdata);
  static size_t WriteHeader(void *buffer, size_t sz, size_t nmemb, void *userdata);
};

#endif /* CHAT_SERVER_HTTPCLIENT_H */
