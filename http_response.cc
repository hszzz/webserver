#include "http_response.h"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace http {

static std::string File2String(const std::string& path) {
  std::ifstream input_file(path, std::ios::in);
  if (!input_file.is_open()) {
    // TODO
  }
  return std::string(std::istreambuf_iterator<char>(input_file),
                     std::istreambuf_iterator<char>());
}

net::Buffer HttpResponse::ToBuffer() const {
  net::Buffer res;

  char status[32];
  ::snprintf(status, sizeof(status), "HTTP/1.1 %d %s \r\n", status_,
             message_.c_str());
  res.append(status);

  if (keepalive_) {
    char content_length[32];
    ::snprintf(content_length, sizeof(content_length),
               "Content-Length: %ld\r\n", body_.size());
    res.append(content_length);
  } else {
    res.append("Connection: close");
  }

  for (const auto& header : headers_) {
    res.append(header.first);
    res.append(": ");
    res.append(header.second);
    res.append("\r\n");
  }

  res.append("\r\n");
  res.append(body_);

  return res;
}

void HttpResponse::SetBodyFromFile(const std::string& path) {
  body_ = File2String(path);
}

}  // namespace http
