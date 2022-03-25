#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

#include "http_parser.h"
#include "http_response.h"
#include "http_server.h"
#include "socket_wrap.h"

namespace http {

void HttpServer::start() {
  fd_ = ::sockets::socket_or_die();
  sockets::socket_reuse(fd_);

  sockaddr_in server;
  ::memset(&server, 0, sizeof(sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = ::htons(port_);
  server.sin_addr.s_addr = INADDR_ANY;
  sockets::bind_or_die(fd_, (sockaddr*)&server);

  sockets::listen_or_die(fd_);

  sockaddr_in client;
  while (1) {
    int cltfd = sockets::accept(fd_, (sockaddr_in*)&client);
    std::thread t(&HttpServer::ThreadFunc, this, cltfd);
    t.detach();
  }
}

void HttpServer::ThreadFunc(int sockfd) {
  char buf[4096] = {0};
  ::read(sockfd, buf, sizeof(buf));
  HttpParser parser;
  std::optional<HttpRequest> req = parser.parser(buf, buf + strlen(buf));
  HttpResponse response;
  if (req.has_value()) {
    HttpRequest request = req.value();
    callback_(&request, &response);
  } else {
    response.SetStatus(HttpStatus::kBadRequest);
    response.SetMessage("Bad Request");
  }
  ::write(sockfd, response.ToBuffer().data(), response.ToBuffer().length());
  ::close(sockfd);
}

}  // namespace http
