#include "BadResponseBuilder.h"
#include "ResponseType.h"
#include <optional>

// no need to encode empty response
Response BadResponseBuilder::build(const Request &req,
                                   const std::optional<std::string> &dir) {
  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");
  return ResponseCreator()
      .setType(ResponseType::Not_Found)
      .setKeepAlive(keepAlive)
      .create();
}