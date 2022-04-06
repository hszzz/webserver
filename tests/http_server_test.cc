#include "../http_server.h"

void HttpCallBack(http::HttpRequest* req, http::HttpResponse* res) {
  if (req->GetMethod() == http::HttpMethod::kGet) {
    if (req->GetPath() == "/") {
      res->SetStatus(http::HttpStatus::kOK);
      res->SetMessage("OK");
      res->SetBody("<h1>hello world</h1>");
    } else if (req->GetPath() == "/ping") {
      res->SetStatus(http::HttpStatus::kOK);
      res->SetMessage("OK");
      res->SetBody("pong");
    } else if (req->GetPath() == "/index.html") {
      res->SetStatus(http::HttpStatus::kOK);
      res->SetMessage("OK");
      res->SetBodyFromFile("/home/hszzz/webserver/docs/index.html");
    } else {
      res->SetStatus(http::HttpStatus::kNotFound);
      res->SetMessage("Page Not Found");
      res->SetBody("Page Not Found!!!");
    }
  } else if (req->GetMethod() == http::HttpMethod::kPost) {
  } else {
    res->SetStatus(http::HttpStatus::kBadRequest);
    res->SetMessage("bad request");
  }
}

int main() {
  http::HttpServer server;
  server.SetPort(8888);
  server.SetHttpCallBack(HttpCallBack);
  server.start();
  return 0;
}
