#include "FileGetResponseBuilder.h"

Response FileGetResponseBuilder::build(const Request &req,
                                       const std::optional<std::string> &dir) {

  std::string filename =
      req.requestLineParts.size() > 1 ? req.requestLineParts[1] : "";

  if (!dir.has_value()) {
    return {"", Internal_Server_Error};
  }

  std::filesystem::path path = dir.value() + filename;
  bool exists =
      (std::filesystem::exists(path) && std::filesystem::is_regular_file(path));

  if (!exists) {
    return {"", Not_Found};
  }
  std::ifstream file(path, std::ios::binary);

  const std::string body = std::string((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());

  return {body, OK, "application/octet-stream"};
}