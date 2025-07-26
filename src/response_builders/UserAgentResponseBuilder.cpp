#include "UserAgentResponseBuilder.h"

std::string UserAgentResponseBuilder::getUserAgent(const std::string &req) {
  std::string pattern = "User-Agent: ";
  auto pos = req.find(pattern);
  std::string user_agent;

  for (int i = pos + pattern.size(); i < req.size(); i++) {
    if (req[i] == '\r') {
      break;
    }
    user_agent += req[i];
  }
  return user_agent;
}

Response
UserAgentResponseBuilder::build(const Request &req,
                                const std::optional<std::string> &dir) {
  std::string body = UserAgentResponseBuilder::getUserAgent(req.req);
  return {body, OK};
}