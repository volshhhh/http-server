#pragma once

#include "Command.h"
#include "RequestType.h"
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Request {
public:
  explicit Request(const std::string &request,
                   std::optional<bool> canHandleFiles = std::nullopt);

  // ? compose all get methods in one

  const RequestType getType() const { return type_; }
  const std::vector<std::string> &getLineParts() const { return lineParts_; }
  const Command getHeadCommand() const { return headCommand_; };
  const std::string &getBody() const { return body_; };
  const std::unordered_map<std::string, std::string> &getHeaders() const {
    return headers_;
  };

private:
  const RequestType type_;
  const std::vector<std::string> lineParts_;
  const Command headCommand_;
  const std::string body_;
  const std::unordered_map<std::string, std::string> headers_;

  const RequestType extractType(const std::string &req);

  const std::vector<std::string> extractLineParts(const std::string &req);

  // TODO: change optional to default value
  const Command extractHead(const std::optional<bool> &canHandleFiles);

  std::string extractBody(const std::string &req);

  const std::unordered_map<std::string, std::string>
  extractHeaders(const std::string &req);
};
