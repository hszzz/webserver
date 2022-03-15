#ifndef SOCKET_WRAP_H
#define SOCKET_WRAP_H

#include <arpa/inet.h>

namespace sockets {

int socket_or_die();
void bind_or_die(int sockfd, const sockaddr* addr);
void listen_or_die(int sockfd);
int accept(int sockfd, sockaddr_in* addr);

}  // namespace sockets

#endif  // SOCKET_WRAP_H
