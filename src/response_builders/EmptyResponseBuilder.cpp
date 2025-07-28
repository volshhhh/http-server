#include "EmptyResponseBuilder.h"
#include "ResponseType.h"

Response EmptyResponseBuilder::build(const Request &req,
                                     const std::optional<std::string> &dir) {
  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");
  return ResponseCreator()
      .setType(ResponseType::OK)
      .setKeepAlive(keepAlive)
      .create();
};