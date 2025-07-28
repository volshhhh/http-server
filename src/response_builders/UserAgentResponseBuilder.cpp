#include "UserAgentResponseBuilder.h"
#include "Response.h"

Response
UserAgentResponseBuilder::build(const Request &req,
                                const std::optional<std::string> &dir) {
  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");

  std::optional<std::string> encoding =
      (req.getHeaders().count("Accept-Encoding") != 0)
          ? std::optional<std::string>(req.getHeaders().at("Accept-Encoding"))
          : std::nullopt;

  const std::string body = req.getHeaders().at("User-Agent");
  return ResponseCreator()
      .setBody(body)
      .setType(ResponseType::OK)
      .setKeepAlive(keepAlive)
      .setContentEncoding(encoding)
      .create();
}