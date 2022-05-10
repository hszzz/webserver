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
    LOG_INFO("new connection");
    std::thread t(&HttpServer::ThreadFunc, this, cltfd);
    t.detach();
  }
}

void HttpServer::ThreadFunc(int sockfd) {
  bool close = true;

  do {
    net::Buffer buffer(4096);
    int err = 0;
    buffer.ReadFd(sockfd, err);
    assert(err == 0);
    HttpParser parser;
    std::optional<HttpRequest> req = parser.parser(
        buffer.BeginRead(), buffer.BeginRead() + buffer.GetReadableBytes());

    HttpResponse response;
    if (req.has_value()) {
      HttpRequest request = req.value();

      std::string connection = request.GetHeader()["Connection"];
      close = connection == "close" ||
              (request.GetVersion() == HttpVersion::kHttp10 &&
               connection != "Keep-Alive");

      response.SetKeepAlive(!close);

      // HTTP 1.0 指定 Connection: keep-alive，保持连接
      // HTTP 1.1 默认 keep-alive，指定 Connection: close 后，断开连接
      //      if (request.GetVersion() == HttpVersion::kHttp10 &&
      //          !request.GetKeepAlive())
      //        response.SetKeepAlive(false);
      //
      //      base::LOG_INFO(request.GetHeader()["Connection"]);
      //      if (request.GetVersion() == HttpVersion::kHttp11 &&
      //          request.GetHeader()["Connection"] == "close")
      //        response.SetKeepAlive(false);

      callback_(&request, &response);
    } else {
      response.SetStatus(HttpStatus::kBadRequest);
      response.SetMessage("Bad Request");
      close = false;
    }
    ::write(sockfd, response.ToBuffer().data(), response.ToBuffer().length());
  } while (close);

  LOG_INFO("disconnect ...");
  ::close(sockfd);
}

}  // namespace http
