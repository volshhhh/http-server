#include "BadResponseBuilder.h"

Response BadResponseBuilder::build(const Request &req,
                                   const std::optional<std::string> &dir) {
  return {"", Not_Found};
}