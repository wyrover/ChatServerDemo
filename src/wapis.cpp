#include <chat_server/wapis.h>
#include <chat_server/HttpClient.h>
#include <chat_server/Logger.h>

namespace {
  const std::string web_service_url = "127.0.0.1:3000";
}

LoginResultPtr AccountService::Login(const std::string& login_name,
                                     const std::string& login_passwd) {
  do {
    HttpClient http;
    const std::string url = web_service_url + "/account/login";
    const std::string para = "login_name=" + login_name + "&login_passwd=" + login_passwd;
    HttpPost post(url);
    post.AddHeader(CONTENT_TYPE_URLENCODED);
    post.SetEntity(para);
    HttpResponsePtr response = http.Execute(post);
    if (response == nullptr) {
      break;
    }
    LoginResultPtr result(new LoginResult());
    std::vector<char>& body_data = response->body_data();
    if (!result->Parse(&*body_data.begin(), body_data.size())) {
      break;
    }
    return result;
  } while (false);
  return nullptr;
}
