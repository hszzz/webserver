#include <stdio.h>

#include "http_response.h"

namespace http {

std::string HttpResponse::ToBuffer() const {
  std::string response;

  char status[32];
  ::snprintf(status, sizeof(status), "HTTP/1.1 %d %s \r\n", status_,
             message_.c_str());
  response += status;

  for (const auto header : headers_) {
    response += header.first;
    response += ": ";
    response += header.second;
    response += "\r\n";
  }

  response += "\r\n";
  response += body_;

  return response;
}

}  // namespace http
