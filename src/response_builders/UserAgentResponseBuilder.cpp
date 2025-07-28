#include "UserAgentResponseBuilder.h"
#include "Response.h"

Response
UserAgentResponseBuilder::build(const Request &req,
                                const std::optional<std::string> &dir) {

  if (req.getHeaders().count("User-Agent") == 0) {
    return {"user-agent not found", Not_Found};
  }
  const std::string body = req.getHeaders().at("User-Agent");
  return {body, OK};
}