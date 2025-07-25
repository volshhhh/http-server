#pragma once

#include "Request.h"
#include "Response.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

// interface
class ResponseBuilder {
public:
  virtual ~ResponseBuilder() = 0;
  virtual Response
  build(const Request &req,
        const std::optional<std::string> &dir = std::nullopt) = 0;
};

inline ResponseBuilder::~ResponseBuilder() {}

class EmptyResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;
};

class EchoResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;
};

class UserAgentResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;

private:
  std::string getUserAgent(const std::string &req);
};

class FileGetResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir) override;
};

class FilePostResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir) override;
};

class BadResponseBuilder : public ResponseBuilder {
public:
  Response build(const Request &req,
                 const std::optional<std::string> &dir = std::nullopt) override;
};