#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <map>
#include <string>
#include <vector>

namespace http {

enum HttpStatus {
  kUnknown = 0,
  kOK = 200,
  kBadRequest = 400,
  kNotFound = 404,
};

class HttpResponse {
 public:
  using HttpHeader = std::map<std::string, std::string>;
  HttpResponse() : status_(kUnknown) {}

  void SetStatus(HttpStatus status) { status_ = status; }
  void SetBody(const std::string& body) { body_ = body; }
  void AddHeader(const std::string& k, const std::string& v) {
    headers_[k] = v;
  }
  void SetMessage(const std::string& message) { message_ = message; }

  std::string ToBuffer() const;

 private:
  HttpHeader headers_;
  HttpStatus status_;
  std::string message_;
  std::string body_;
};

}  // namespace http

#endif  // HTTP_RESPONSE_H
