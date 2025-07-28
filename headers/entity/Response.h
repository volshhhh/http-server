#pragma once

#include "Request.h"
#include <string>
#include <vector>

enum ResponseType {
  OK = 200,
  Created = 201,
  Not_Found = 404,
  Internal_Server_Error = 500
};

enum ResponseHeader {
  Content_Encoding,
  Content_Type,
  Content_Lenght,
  Connection,

  ENUM_SIZE // dummy element
};

// TODO: addHeader logic (also cheeck contect encoding)
struct Response {
  std::string body_;
  ResponseType type_;
  std::string contentType_;
  bool keepAlive_;

  Response(const std::string &body, ResponseType type,
           const std::string &contentType = "text/plain", bool keepAlive = true)
      : presentHeaders(ResponseHeader::ENUM_SIZE, false), body_(body),
        type_(type), contentType_(contentType), keepAlive_(keepAlive) {

    addHeader(ResponseHeader::Content_Type);
    addHeader(ResponseHeader::Connection);
    if (type_ == ResponseType::Not_Found) {
      canAddHeader = false;
    }
    addHeader(ResponseHeader::Content_Lenght);
  };

  const std::string to_string();

private:
  std::vector<bool> presentHeaders;
  bool canAddHeader = true;

  void addHeader(ResponseHeader header);
};
