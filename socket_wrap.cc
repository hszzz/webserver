#include "socket_wrap.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

namespace sockets {

int socket_or_die() {
  int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    ::exit(-1);
  }
  return sockfd;
}

void bind_or_die(int sockfd, const sockaddr* addr) {
  int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in)));
  if (ret < 0) ::exit(-1);
}

void listen_or_die(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) ::exit(-1);
}

int accept(int sockfd, sockaddr_in* addr) {
  socklen_t addrlen = static_cast<socklen_t>(sizeof(sockaddr_in));
  int connfd = ::accept(sockfd, (sockaddr*)addr, &addrlen);
  if (connfd <= 0) {
    // TODO
  }
  return connfd;
}

} // namespace sockets

