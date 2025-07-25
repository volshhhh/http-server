#pragma once

#include <string>

enum ResponseType {
  OK = 200,
  Created = 201,
  Not_Found = 404,
  Internal_Server_Error = 500
};

inline std::string to_string(ResponseType type) {
  switch (type) {
  case ResponseType::OK:
    return "200 OK\r\n";
  case ResponseType::Not_Found:
    return "404 Not Found\r\n";
  case ResponseType::Internal_Server_Error:
    return "500 Internal Error\r\n";
  default:
    return "Unknown\r\n";
  }
}

struct Response {
  std::string body;
  ResponseType type = OK;
  std::string content_type = "text/plain";
};
