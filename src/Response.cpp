#include "Response.h"
#include <magic_enum.hpp>

Response::Response(const std::string &body, ResponseType type,
                   const std::string &contentType, bool keepAlive,
                   const std::optional<std::string> &contentEncoding)
    : presentHeaders(magic_enum::enum_count<ResponseHeader>(), false),
      body_(body), type_(type), contentType_(contentType),
      keepAlive_(keepAlive), contentEncoding_(contentEncoding) {

  addHeader(ResponseHeader::Content_Type);
  addHeader(ResponseHeader::Connection);
  addHeader(ResponseHeader::Content_Length);

  if (type_ == ResponseType::Not_Found) {
    canAddHeader = false;
  }

  if (contentEncoding_.has_value()) {
    addHeader(ResponseHeader::Content_Encoding);
  }
};

void Response::addHeader(ResponseHeader header) {
  if (canAddHeader) {
    presentHeaders[static_cast<size_t>(header)] = true;
  }
}

std::string Response::getHeaderContent(ResponseHeader header) {
  switch (header) {
  case ResponseHeader::Content_Encoding:
    return (contentEncoding_.has_value()) ? contentEncoding_.value() : "none";
  case ResponseHeader::Content_Type:
    return contentType_;
  case ResponseHeader::Content_Length:
    return std::to_string(body_.size());
  case ResponseHeader::Connection:
    return (keepAlive_) ? "keep-alive" : "close";
  }
}

const std::string Response::to_string() {
  std::string response = "HTTP/1.1 " + toString(type_) + "\r\n";
  for (size_t i = 0; i < presentHeaders.size(); i++) {
    if (presentHeaders[i]) {
      ResponseHeader header = magic_enum::enum_value<ResponseHeader>(i);
      response += toString(header);
      response += ": ";
      response += getHeaderContent(header);
      response += "\r\n";
    }
  }
  response += "\r\n" + body_;
  return response;
}