#pragma once

#include "ResponseBuilder.h"

class EmptyResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;
};