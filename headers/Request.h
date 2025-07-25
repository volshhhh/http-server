#pragma once

#include "Command.h"
#include "RequestType.h"
#include <optional>
#include <string>
#include <vector>

struct Request {
  const std::string req;
  const RequestType reqType;
  const std::vector<std::string> requestLineParts;
  const Command headCommand;
  const std::string requestBody;

  explicit Request(const std::string &request,
                   std::optional<bool> canHandleFiles = std::nullopt);

private:
  const RequestType getRequestType(const std::string &req);

  const std::vector<std::string> parseRequest(const std::string &req);

  std::string extractRequestBody(const std::string &rawRequest);

  const Command determineHead(const std::optional<bool> &canHandleFiles);
};
