#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

#include "buffer.h"
#include "http_parser.h"
#include "http_response.h"
#include "http_server.h"
#include "log.h"
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
    char ip[16] = {0};
    ::inet_ntop(AF_INET, &client.sin_addr.s_addr, ip, sizeof(client));

    char log[128];

    snprintf(log, sizeof(log), "new connection: %s:%d ...", ip,
             client.sin_port);

    LOG_INFO(log);
    std::thread t(&HttpServer::ThreadFunc, this, cltfd);
    t.detach();
  }
}

void HttpServer::ThreadFunc(int sockfd) {
  bool keep = false;
  do {
    net::Buffer buffer(4096);
    int err = 0;
    buffer.ReadFd(sockfd, err);
    assert(err == 0);
    HttpParser parser;
    HttpResponse res;
    std::optional<HttpRequest> req =
        parser.parser(buffer.peek(), buffer.peek() + buffer.GetReadableBytes());

    if (req.has_value()) {
      if (req.value().GetVersion() == kHttp10 && req.value().GetKeepAlive()) {
        res.SetKeepAlive(true);
        keep = true;
      }

      if (req.value().GetVersion() == kHttp11 &&
          req.value().GetHeader()["Connection"] != "close") {
        res.SetKeepAlive(true);
        keep = true;
      }

      callback_(&req.value(), &res);
    }

    ::write(sockfd, res.ToBuffer().peek(), res.ToBuffer().GetWritableBytes());
  } while (keep);
  LOG_INFO("disconnect ...");
  ::close(sockfd);
}

}  // namespace http
