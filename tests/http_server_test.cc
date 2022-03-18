#include "../http_server.h"

int main() {
  http::HttpServer server;
  server.SetPort(8888);
  server.start();
  return 0;
}

