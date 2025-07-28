#include "Request.h"
#include "Command.h"
#include "RequestType.h"
#include <iostream>

Request::Request(const std::string &request,
                 const std::optional<bool> canHandleFiles)
    : type_(extractType(request)), lineParts_(extractLineParts(request)),
      headCommand_(extractHead(canHandleFiles)), body_(extractBody(request)),
      headers_(extractHeaders(request)) {}

const RequestType Request::extractType(const std::string &req) {
  auto firstSlash = req.find("/");
  std::string reqType = req.substr(0, firstSlash - 1);

  if (reqType == "POST") {
    return RequestType::POST;
  }

  return RequestType::GET;
}

const std::vector<std::string>
Request::extractLineParts(const std::string &req) {
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

std::string Request::extractBody(const std::string &rawRequest) {
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

const Command Request::extractHead(const std::optional<bool> &canHandleFiles) {
  if (lineParts_.empty()) {
    std::cout << "EMPTYY" << std::endl;
    return Command::EMPTY;
  }
  std::string strHead = lineParts_[0];

  if (strHead == "echo") {
    std::cout << "ECHOOO" << std::endl;
    return Command::ECHO;

  } else if (strHead == "user-agent") {
    std::cout << "USER-AGENT" << std::endl;
    return Command::USER_AGENT;

  } else if (canHandleFiles.has_value() && canHandleFiles.value() &&
             strHead == "files") {

    if (type_ == RequestType::GET) {
      std::cout << "GET-FILES" << std::endl;
      return Command::GET_FILES;
    }
    std::cout << "POST-FILES" << std::endl;
    return Command::POST_FILES;
  }

  std::cout << "FAIL" << std::endl;

  return Command::FAIL;
}

const std::unordered_map<std::string, std::string>
Request::extractHeaders(const std::string &req) {
  std::unordered_map<std::string, std::string> headers;

  size_t start_pos = req.find("\r\n") + 2;
  size_t end_pos = req.find("\r\n\r\n");

  if (end_pos == std::string::npos) {
    end_pos = req.length();
  }

  std::string headers_block = req.substr(start_pos, end_pos - start_pos);
  size_t line_start = 0;

  while (line_start < headers_block.length()) {
    size_t line_end = headers_block.find("\r\n", line_start);
    if (line_end == std::string::npos) {
      line_end = headers_block.length();
    }

    std::string header_line =
        headers_block.substr(line_start, line_end - line_start);
    size_t colon_pos = header_line.find(':');

    if (colon_pos != std::string::npos) {
      std::string key = header_line.substr(0, colon_pos);
      std::string value = header_line.substr(colon_pos + 1);

      key.erase(0, key.find_first_not_of(" \t"));
      key.erase(key.find_last_not_of(" \t") + 1);
      value.erase(0, value.find_first_not_of(" \t"));
      value.erase(value.find_last_not_of(" \t") + 1);

      headers[key] = value;
    }

    line_start = line_end + 2;
  }
  return headers;
};