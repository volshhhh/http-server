#include "ResponseFactory.h"
#include "ResponseBuilder.h"

using BuilderCreator = std::function<std::unique_ptr<ResponseBuilder>()>;

void ResponseBuilderFactory::registerBuilder(Command cmd,
                                             BuilderCreator creator) {
  builders_[cmd] = creator;
}

ResponseBuilderFactory::ResponseBuilderFactory(std::optional<std::string> &dir)
    : fileDir(dir) {
  registerBuilder(EMPTY,
                  []() { return std::make_unique<EmptyResponseBuilder>(); });

  registerBuilder(ECHO,
                  []() { return std::make_unique<EchoResponseBuilder>(); });

  if (dir.has_value()) {
    registerBuilder(
        GET_FILES, []() { return std::make_unique<FileGetResponseBuilder>(); });

    registerBuilder(POST_FILES, []() {
      return std::make_unique<FilePostResponseBuilder>();
    });
  }
  registerBuilder(USER_AGENT, []() {
    return std::make_unique<UserAgentResponseBuilder>();
  });

  registerBuilder(FAIL,
                  []() { return std::make_unique<BadResponseBuilder>(); });
}

Response
ResponseBuilderFactory::getResponse(const Request &req,
                                    const std::optional<std::string> &dir) {

  auto it = builders_.find(req.getHeadCommand());
  return (it->second())->build(req, dir);
}
