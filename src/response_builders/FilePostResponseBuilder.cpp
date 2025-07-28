#include "FilePostResponseBuilder.h"

Response FilePostResponseBuilder::build(const Request &req,
                                        const std::optional<std::string> &dir) {
  if (req.getLineParts().size() == 1) {
    return {"", Not_Found};
  }
  const std::string content = req.getBody();
  const std::string filename = req.getLineParts()[1];

  if (!dir.has_value()) {
    return {"", Internal_Server_Error};
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
      std::cerr << "Failed to create or open file" << std::endl;
      return {"failed to create", Not_Found};
    }
  }

  return {"", Created, "application/octet-stream"};
}