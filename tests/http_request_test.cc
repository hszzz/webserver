#include "../http_request.h"

#include <iostream>

using namespace http;

void debug(const HttpRequest& request) {
  std::cout << request.GetMethodString() << " " << request.GetVersionString() << " " 
    << request.GetPath() << request.GetQuery() << std::endl;
  for (const auto& header : request.GetHeader()) 
    std::cout << header.first << ": " << header.second << std::endl;
  std::cout << std::endl;
}

int main() {
  HttpRequest req;
  debug(req);

  req.SetMethod("GET");
  req.SetVersion("HTTP/1.0");
  req.SetPath("/");
  req.SetQuery("?d=1");

  req.AddHeader("Host", "127.0.0.1");
  req.AddHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36");
  req.AddHeader("Date", "test");
  debug(req);

  return 0;
}

