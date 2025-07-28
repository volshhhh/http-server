#pragma once

#include "Request.h"
#include "ResponseHeader.h"
#include "ResponseType.h"
#include <magic_enum.hpp>
#include <optional>
#include <string>
#include <vector>

struct Response {
  std::string body_;
  ResponseType type_;
  std::string contentType_;
  std::optional<std::string> contentEncoding_;
  bool keepAlive_;

  Response(const std::string &body, ResponseType type,
           const std::string &contentType = "text/plain", bool keepAlive = true,
           const std::optional<std::string> &contentEncoding = std::nullopt);

  const std::string to_string();

private:
  std::vector<bool> presentHeaders;
  bool canAddHeader = true;

  void addHeader(ResponseHeader header);

  std::string getHeaderContent(ResponseHeader header);
};
