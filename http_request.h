#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <map>
#include <string>
#include <string_view>

namespace http {

enum HttpMethod { kInvalid = 0, kGet, kPost };
enum HttpVersion { kUnknow = 0, kHttp10, kHttp11 };

class HttpRequest {
 public:
  using HttpHeader = std::map<std::string, std::string>;
  HttpRequest() : method_(kInvalid), version_(kUnknow) {}

  void SetMethod(std::string_view method);
  HttpMethod GetMethod() const { return method_; }
  std::string GetMethodString() const;

  void SetVersion(std::string_view version);
  HttpVersion GetVersion() const { return version_; }
  std::string GetVersionString() const;

  void SetPath(std::string_view path) { path_ = path; }
  std::string GetPath() const { return path_; };

  void SetQuery(std::string_view query) { query_ = query; }
  std::string GetQuery() const { return query_; };

  void AddHeader(std::string k, std::string v) { headers_.insert({k, v}); }
  HttpHeader GetHeader() const { return headers_; };

 private:
  HttpMethod method_;
  HttpVersion version_;
  std::string path_;
  std::string query_;
  HttpHeader headers_;
};

}  // namespace http

#endif  // HTTP_REQUEST_H
