#include "../http_server.h"

void HttpCallBack(http::HttpRequest* req, http::HttpResponse* res) {
  if (req->GetMethod() == http::HttpMethod::kGet) {
    res->SetStatus(http::HttpStatus::kOK);
    res->SetMessage("200 OK");
    res->SetBody("<h1>hello world</h1>");
  }
}

int main() {
  http::HttpServer server;
  server.SetPort(8888);
  server.SetHttpCallBack(HttpCallBack);
  server.start();
  return 0;
}
