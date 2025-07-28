#include "FileGetResponseBuilder.h"

Response FileGetResponseBuilder::build(const Request &req,
                                       const std::optional<std::string> &dir) {
  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");

  std::optional<std::string> encoding =
      (req.getHeaders().count("Accept-Encoding") != 0)
          ? std::optional<std::string>(req.getHeaders().at("Accept-Encoding"))
          : std::nullopt;

  std::string filename =
      req.getLineParts().size() > 1 ? req.getLineParts()[1] : "";

  if (!dir.has_value()) {
    return ResponseCreator()
        .setType(ResponseType::Internal_Server_Error)
        .setKeepAlive(keepAlive)
        .create();
  }

  std::filesystem::path path = dir.value() + filename;
  bool exists =
      (std::filesystem::exists(path) && std::filesystem::is_regular_file(path));

  if (!exists) {
    return ResponseCreator()
        .setType(ResponseType::Not_Found)
        .setKeepAlive(keepAlive)
        .create();
  }
  std::ifstream file(path, std::ios::binary);

  const std::string body = std::string((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());

  return ResponseCreator()
      .setBody(body)
      .setType(ResponseType::Not_Found)
      .setContentType("application/octet-stream")
      .setContentEncoding(encoding)
      .create();
}