#include "FilePostResponseBuilder.h"
#include "ResponseType.h"

Response FilePostResponseBuilder::build(const Request &req,
                                        const std::optional<std::string> &dir) {

  bool keepAlive = !(req.getHeaders().count("Connection") != 0 &&
                     req.getHeaders().at("Connection") == "close");

  std::optional<std::string> encoding =
      (req.getHeaders().count("Accept-Encoding") != 0)
          ? std::optional<std::string>(req.getHeaders().at("Accept-Encoding"))
          : std::nullopt;

  if (req.getLineParts().size() == 1) {
    return ResponseCreator().setType(ResponseType::Not_Found).create();
  }
  const std::string content = req.getBody();
  const std::string filename = req.getLineParts()[1];

  if (!dir.has_value()) {
    return ResponseCreator()
        .setBody("directory not found")
        .setType(ResponseType::Not_Found)
        .setKeepAlive(keepAlive)
        .create();
  }

  {
    namespace fs = std::filesystem;

    fs::path filePath = dir.value() + filename;

    if (!fs::exists(filePath.parent_path())) {
      fs::create_directories(filePath.parent_path());
    }

    std::ofstream file(filePath);
    if (file.is_open()) {
      file << content;
      file.close();
    } else {
      return ResponseCreator()
          .setBody("failed to create or open")
          .setType(ResponseType::Not_Found)
          .setKeepAlive(keepAlive)
          .create();
    }
  }

  return ResponseCreator()
      .setBody("succesfully created")
      .setType(ResponseType::Created)
      .setContentType("application/octet-stream")
      .setKeepAlive(keepAlive)
      .setContentEncoding(encoding)
      .create();
}