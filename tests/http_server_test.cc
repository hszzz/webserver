#include "../http_server.h"

void HttpCallBack(http::HttpRequest* req, http::HttpResponse* res) {
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
  }
}

int main() {
  http::HttpServer server;
  server.SetPort(8888);
  server.SetHttpCallBack(HttpCallBack);
  server.start();
  return 0;
}
