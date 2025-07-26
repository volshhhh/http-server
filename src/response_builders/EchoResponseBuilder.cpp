#include "EchoResponseBuilder.h"

Response EchoResponseBuilder::build(const Request &req,
                                    const std::optional<std::string> &dir) {

  const std::string body =
      (req.requestLineParts.size() > 1) ? req.requestLineParts[1] : "";
  return {body, OK};
}
