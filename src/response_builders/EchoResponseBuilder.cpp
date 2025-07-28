#include "EchoResponseBuilder.h"
#include "Response.h"

Response EchoResponseBuilder::build(const Request &req,
                                    const std::optional<std::string> &dir) {

  const std::string body =
      (req.getLineParts().size() > 1) ? req.getLineParts()[1] : "";

  return {body, OK};
}
