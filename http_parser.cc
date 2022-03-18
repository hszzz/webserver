#include <string.h>

#include <algorithm>
#include <iostream>

#include "http_parser.h"

namespace http {

static const char* find(const char* start, const char* end,
                        std::string_view pattern) {
  /*
  const char* ret =
      (const char*)::memmem((const void*)start, std::distance(end, start),
                            (const void*)pattern.begin(), pattern.size());
  */
  const char* ret = std::search(start, end, pattern.begin(), pattern.end());
  return ret == end ? nullptr : ret;
}

static std::vector<std::string_view> split(std::string_view line,
                                           const char d) {
  std::vector<std::string_view> ret;

  const char* begin = line.begin();
  size_t count = 0;
  for (const char* it = line.begin(); it != line.end(); ++it) {
    if (*it != d)
      count++;
    else {
      ret.push_back(std::string_view(begin, count));
      count = 0;
      begin = it + 1;
    }
  }
  if (count) ret.push_back(std::string_view(begin, count));

  return ret;
}

bool ParseResquestLine(const char* start, const char* end,
                       HttpRequest* request) {
  bool ok = false;
  std::vector<std::string_view> s =
      split(std::string_view(start, end - start), ' ');
  if (s.size() == 3) {
    request->SetMethod(s[0]);
    request->SetVersion(s[2]);
    auto pq = split(s[1], '?');
    if (pq.size() == 2) {
      request->SetPath(pq[0]);
      request->SetQuery(pq[1]);
    } else {
      request->SetPath(s[1]);
    }
    ok = true;
  }

  return ok;
}

std::optional<HttpRequest> HttpParser::parser(const char* start,
                                              const char* end) {
  HttpRequest request;

  const char* crlf = find(start, end, "\r\n");
  if (crlf == nullptr) return {};

  bool ok = ParseResquestLine(start, crlf, &request);
  if (!ok) return {};

  state_ = kRequestHeader;
  while (state_ == kRequestHeader) {
    const char* s = crlf + 2;
    crlf = find(s, end, "\r\n");
    if (crlf) {
      const char* colon = find(s, crlf, ":");
      if (colon) {
        request.AddHeader(std::string(s, colon - s),
                          std::string(colon + 2, crlf - colon - 2));
      } else {
        state_ = kRequestBody;
        break;
      }
    } else {
      state_ = kRequestBody;
    }
  }

  return request;
}

}  // namespace http
