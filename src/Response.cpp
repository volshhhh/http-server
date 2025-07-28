#include "Response.h"

void Response::addHeader(ResponseHeader header) {
  if (header != ResponseHeader::ENUM_SIZE && canAddHeader) {
    presentHeaders[header] = true;
  }
}

const std::string Response::to_string() {
  std::string status_text;
  switch (type_) {
  case 200:
    status_text = "OK";
    break;
  case 201:
    status_text = "Created";
    break;
  case 404:
    status_text = "Not Found";
    break;
  case 500:
    status_text = "Internal Error";
    break;
  default:
    status_text = "Unknown";
    break;
  }

  std::string connection = (keepAlive_) ? "keep-alive" : "close";

  std::string response =
      "HTTP/1.1 " + std::to_string(type_) + " " + status_text + "\r\n" +
      "Content-Type: " + contentType_ + "\r\n" +
      "Content-Length: " + std::to_string(body_.size()) + "\r\n" +
      "Connection: " + connection + "\r\n" + "\r\n" + body_;

  return response;
}