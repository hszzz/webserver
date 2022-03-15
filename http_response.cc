#include <stdio.h>

#include "http_response.h"

namespace http {

std::string HttpResponse::ToBuffer() {
  std::string response;

  ::snprintf(response.data(), response.size(), "%s ");

  return response;
}

}  // namespace http
