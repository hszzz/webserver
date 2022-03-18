#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <optional>
#include <string_view>

#include "http_request.h"

namespace http {

enum HttpParserState {
  kRequestLine = 0,
  kRequestHeader,
  kRequestBody,
  kGotAll,
};

class HttpParser {
 public:
  HttpParser() : state_(kRequestLine) {}

  HttpParserState state() const { return state_; };
  std::optional<HttpRequest> parser(const char* start, const char* end);

 private:
  HttpParserState state_;
};

}  // namespace http

#endif  // HTTP_PARSER_H
