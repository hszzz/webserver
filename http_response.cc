#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "http_response.h"

namespace http {

static std::string File2String(const std::string& path) {
  std::ifstream input_file(path, std::ios::in);
  if (!input_file.is_open()) {
    // TODO
  }
  return std::string(std::istreambuf_iterator<char>(input_file),
                     std::istreambuf_iterator<char>());
}

std::string HttpResponse::ToBuffer() const {
  std::string response;

  char status[32];
  ::snprintf(status, sizeof(status), "HTTP/1.1 %d %s \r\n", status_,
             message_.c_str());
  response += status;

  if (keepalive_) {
    response += "Connection: Keep-Alive\r\n";
    char content_length[32];
    ::snprintf(content_length, sizeof(content_length),
               "Content-Length: %ld\r\n", body_.size());
    response += content_length;
  } else {
    response += "Connection: close\r\n";
  }

  for (const auto& header : headers_) {
    response += header.first;
    response += ": ";
    response += header.second;
    response += "\r\n";
  }

  response += "\r\n";
  response += body_;

  return response;
}

void HttpResponse::SetBodyFromFile(const std::string& path) {
  body_ = File2String(path);
}

}  // namespace http
