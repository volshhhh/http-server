#pragma once

#include "ResponseHeader.h"
#include "ResponseType.h"
#include <magic_enum.hpp>
#include <optional>
#include <string>
#include <vector>

struct Response {
  const std::string to_string();

  const std::string getBody() { return body_; }
  const ResponseType getResponseType() { return type_; }
  const std::string getContentType() { return contentType_; }
  const bool keepAlive() { return keepAlive_; }

  friend class ResponseCreator;

private:
  std::string body_;
  ResponseType type_;
  std::string contentType_;
  std::optional<std::string> contentEncoding_;
  bool keepAlive_;

  Response(const std::string &body, ResponseType type,
           const std::string &contentType = "text/plain", bool keepAlive = true,
           const std::optional<std::string> &contentEncoding = std::nullopt);
  std::vector<bool> presentHeaders;
  bool canAddHeader = true;

  void addHeader(ResponseHeader header);

  std::string getHeaderContent(ResponseHeader header);
};

class ResponseCreator {
public:
  ResponseCreator &setBody(const std::string &body) {
    body_ = body;
    return *this;
  }

  ResponseCreator &setType(ResponseType type) {
    type_ = type;
    return *this;
  }

  ResponseCreator &setContentType(const std::string &contentType) {
    contentType_ = contentType;
    return *this;
  }

  ResponseCreator &setKeepAlive(bool keepAlive) {
    keepAlive_ = keepAlive;
    return *this;
  }

  ResponseCreator &
  setContentEncoding(const std::optional<std::string> &encoding) {
    contentEncoding_ = encoding;
    return *this;
  }

  Response create() {
    return Response(body_, type_, contentType_, keepAlive_, contentEncoding_);
  }

private:
  std::string body_ = "";
  ResponseType type_ = ResponseType::Not_Found;
  std::string contentType_ = "text/plain";
  bool keepAlive_ = true;
  std::optional<std::string> contentEncoding_ = std::nullopt;
};
