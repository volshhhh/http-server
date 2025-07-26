#pragma once

#include "ResponseBuilder.h"

class UserAgentResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;

private:
  std::string getUserAgent(const std::string &req);
};