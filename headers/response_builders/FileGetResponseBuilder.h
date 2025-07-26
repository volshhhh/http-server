#pragma once

#include "ResponseBuilder.h"

class FileGetResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir) override;
};