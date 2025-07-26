#pragma once

#include "Request.h"
#include "Response.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

// interface for response builders
class ResponseBuilder {
public:
  virtual ~ResponseBuilder() = 0;
  virtual Response
  build(const Request &req,
        const std::optional<std::string> &dir = std::nullopt) = 0;
};

inline ResponseBuilder::~ResponseBuilder() {}