#pragma once

#include "Command.h"
#include "Request.h"
#include "ResponseBuilder.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "BadResponseBuilder.h"
#include "EchoResponseBuilder.h"
#include "EmptyResponseBuilder.h"
#include "FileGetResponseBuilder.h"
#include "FilePostResponseBuilder.h"
#include "UserAgentResponseBuilder.h"

class ResponseBuilderFactory {
private:
  using BuilderCreator = std::function<std::unique_ptr<ResponseBuilder>()>;
  std::unordered_map<const Command, BuilderCreator> builders_;

  void registerBuilder(Command cmd, BuilderCreator creator);

  std::optional<std::string> fileDir;

public:
  ResponseBuilderFactory() = default;

  ResponseBuilderFactory(std::optional<std::string> &dir);

  Response getResponse(const Request &req,
                       const std::optional<std::string> &dir = std::nullopt);
};
