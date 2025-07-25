#include "ResponseBuilder.h"

Response EmptyResponseBuilder::build(const Request &req,
                                     const std::optional<std::string> &dir) {
  return {"", OK};
};

Response EchoResponseBuilder::build(const Request &req,
                                    const std::optional<std::string> &dir) {

  const std::string body =
      (req.requestLineParts.size() > 1) ? req.requestLineParts[1] : "";
  return {body, OK};
}

std::string UserAgentResponseBuilder::getUserAgent(const std::string &req) {
  std::string pattern = "User-Agent: ";
  auto pos = req.find(pattern);
  std::string user_agent;

  for (int i = pos + pattern.size(); i < req.size(); i++) {
    if (req[i] == '\r') {
      break;
    }
    user_agent += req[i];
  }
  return user_agent;
}

Response
UserAgentResponseBuilder::build(const Request &req,
                                const std::optional<std::string> &dir) {
  std::string body = UserAgentResponseBuilder::getUserAgent(req.req);
  return {body, OK};
}

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

Response FilePostResponseBuilder::build(const Request &req,
                                        const std::optional<std::string> &dir) {
  if (req.requestLineParts.size() == 1) {
    return {"", Not_Found};
  }
  const std::string content = req.requestBody;
  const std::string filename = req.requestLineParts[1];

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

Response BadResponseBuilder::build(const Request &req,
                                   const std::optional<std::string> &dir) {
  return {"", Not_Found};
}