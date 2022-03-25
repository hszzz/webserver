#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <functional>

#include "http_request.h"
#include "http_response.h"

namespace http {

class HttpServer {
 public:
  using HttpCallBack = std::function<void(HttpRequest*, HttpResponse*)>;

  HttpServer() : fd_(0), port_(8080) {}
  void SetPort(int port) { port_ = port; }
  void start();

  void SetHttpCallBack(HttpCallBack callback) { callback_ = callback; }

 private:
  void ThreadFunc(int sockfd);

 private:
  int fd_;
  int port_;
  HttpCallBack callback_;
};

}  // namespace http

#endif  // HTTP_SERVER_H
