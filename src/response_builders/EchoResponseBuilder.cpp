#include "EchoResponseBuilder.h"
#include "Response.h"
#include "ResponseType.h"

Response EchoResponseBuilder::build(const Request &req,
                                    const std::optional<std::string> &dir) {

  const std::string body =
      (req.getLineParts().size() > 1) ? req.getLineParts()[1] : "";

  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");
  std::optional<std::string> encoding =
      (req.getHeaders().count("Accept-Encoding") != 0)
          ? std::optional<std::string>(req.getHeaders().at("Accept-Encoding"))
          : std::nullopt;

  return ResponseCreator()
      .setBody(body)
      .setType(ResponseType::OK)
      .setKeepAlive(keepAlive)
      .setContentEncoding(encoding)
      .create();
}
