#include <string.h>
#include <unistd.h>

#include <iostream>
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
    int c = sockets::accept(fd_, &client);
    char buf[1024];
    ::read(c, buf, sizeof(buf));
    std::cout << buf << std::endl;

    HttpParser parser;
    std::optional<HttpRequest> req = parser.parser(buf, buf + strlen(buf));
    if (req.has_value()) {
      std::cout << "method: " << req.value().GetMethod() << std::endl
                << "path: " << req.value().GetPath() << std::endl
                << "query: " << req.value().GetQuery() << std::endl
                << "version: " << req.value().GetVersion() << std::endl;
      std::cout << "headers: " << std::endl;
      for (const auto& kv : req.value().GetHeader()) {
        std::cout << kv.first << ": " << kv.second << std::endl;
      }
    }

    HttpResponse response;
    response.SetStatus(kOK);
    response.SetMessage("i am not ok");
    response.AddHeader("Server", "hszzz-toy-server");
    auto res = response.ToBuffer() + "<h1>TEST TEST TEST</h1>";
    ::write(c, res.c_str(), res.length());
    ::close(c);
  }
}

}  // namespace http
