#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "log.h"
#include "socket_wrap.h"

namespace sockets {

int socket_or_die() {
  int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) LOG_FATAL("socket error");
  return sockfd;
}

void bind_or_die(int sockfd, const sockaddr* addr) {
  int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in)));
  if (ret < 0) LOG_FATAL("bind error");
}

void listen_or_die(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) LOG_FATAL("listen error");
}

int accept(int sockfd, sockaddr_in* addr) {
  socklen_t addrlen = static_cast<socklen_t>(sizeof(sockaddr_in));
  int connfd = ::accept(sockfd, (sockaddr*)addr, &addrlen);
  if (connfd <= 0) LOG_FATAL("accept error");
  return connfd;
}

void socket_reuse(int sockfd) {
  int reuse = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&reuse,
                   sizeof(int)) < 0) {
    LOG_FATAL("socket_reuse error");
  }
}

}  // namespace sockets
