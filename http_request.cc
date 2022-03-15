#include "http_request.h"

namespace http {

void HttpRequest::SetMethod(std::string_view method) {
#define XX(m, v)      \
  if (method == #v) { \
    method_ = m;      \
    return;           \
  }

  XX(kGet, GET);
  XX(kPost, POST);

#undef XX
  method_ = kInvalid;
}

void HttpRequest::SetVersion(std::string_view version) {
#define XX(ver, v)    \
  if (version == v) { \
    version_ = ver;   \
    return;           \
  }

  XX(kHttp10, "HTTP/1.0");
  XX(kHttp11, "HTTP/1.1");

#undef XX
  version_ = kUnknow;
}

std::string HttpRequest::GetMethodString() const {
  switch (method_) {
#define XX(method, v) \
  case method:        \
    return #v;        \
    break;

    XX(kGet, GET);
    XX(kPost, POST);

#undef XX
    default:
      return "INVAILD";
  }

  return "INVAILD";
}

std::string HttpRequest::GetVersionString() const {
  switch (version_) {
#define XX(ver, v) \
  case ver:        \
    return v;      \
    break;

    XX(kHttp10, "HTTP/1.0");
    XX(kHttp11, "HTTP/1.1");

#undef XX
    default:
      return "UNKNOWN";
  }

  return "UNKNOWN";
}

}  // namespace http
