#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

namespace http {

class HttpServer {
 public:
  HttpServer() : fd_(0), port_(8080) {}
  void SetPort(int port) { port_ = port; }

  void start();

 private:
  int fd_;
  int port_;
};

}  // namespace http

#endif  // HTTP_SERVER_H

