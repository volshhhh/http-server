#include "EmptyResponseBuilder.h"

Response EmptyResponseBuilder::build(const Request &req,
                                     const std::optional<std::string> &dir) {
  return {"", OK};
};