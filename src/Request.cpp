#include "Request.h"
#include <iostream>

Request::Request(const std::string &request,
                 const std::optional<bool> canHandleFiles)
    : req(request), reqType(getRequestType(req)),
      requestLineParts(parseRequest(request)),
      headCommand(determineHead(canHandleFiles)),
      requestBody(extractRequestBody(request)) {}

const RequestType Request::getRequestType(const std::string &req) {
  auto firstSlash = req.find("/");
  std::string reqType = req.substr(0, firstSlash - 1);

  if (reqType == "POST") {
    return RequestType::POST;
  }

  return RequestType::GET;
}

const std::vector<std::string> Request::parseRequest(const std::string &req) {
  auto begin = req.find("/") + 1;
  auto end = req.find("HTTP");
  std::string curr;
  std::vector<std::string> parts;

  for (size_t i = begin; i < end - 1; i++) {
    if (req[i] == '/' && !curr.empty()) {
      parts.push_back(curr);
      curr.clear();
    } else {
      curr += req[i];
    }
  }

  if (!curr.empty()) {
    parts.push_back(curr);
  }
  return parts;
}

std::string Request::extractRequestBody(const std::string &rawRequest) {
  size_t headerEnd = rawRequest.find("\r\n\r\n");
  if (headerEnd == std::string::npos) {
    return "";
  }

  size_t bodyStart = headerEnd + 4;
  std::string body = rawRequest.substr(bodyStart);

  size_t contentLengthPos = rawRequest.find("Content-Length: ");
  if (contentLengthPos != std::string::npos) {
    size_t lengthStart = contentLengthPos + 16;
    size_t lengthEnd = rawRequest.find("\r\n", lengthStart);
    if (lengthEnd != std::string::npos) {
      std::string lengthStr =
          rawRequest.substr(lengthStart, lengthEnd - lengthStart);
      int contentLength = std::stoi(lengthStr);
      body = body.substr(0, contentLength);
    }
  }

  return body;
}

const Command
Request::determineHead(const std::optional<bool> &canHandleFiles) {
  if (requestLineParts.empty()) {
    std::cout << "EMPTYY" << std::endl;
    return EMPTY;
  }
  std::string strHead = requestLineParts[0];

  if (strHead == "echo") {
    std::cout << "ECHOOO" << std::endl;
    return ECHO;

  } else if (strHead == "user-agent") {
    std::cout << "USER-AGENT" << std::endl;
    return USER_AGENT;

  } else if (canHandleFiles.has_value() && canHandleFiles.value() &&
             strHead == "files") {

    if (reqType == GET) {
      std::cout << "GET-FILES" << std::endl;
      return GET_FILES;
    }
    std::cout << "POST-FILES" << std::endl;
    return POST_FILES;
  }

  std::cout << "FAIL" << std::endl;

  return FAIL;
}