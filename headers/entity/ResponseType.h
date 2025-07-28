#pragma once

#include <algorithm>
#include <magic_enum.hpp>
#include <string>

enum ResponseType {
  OK = 200,
  Created = 201,
  Not_Found = 404,
  Internal_Server_Error = 500
};

inline std::string toString(ResponseType type) {
  std::string result = static_cast<std::string>(magic_enum::enum_name(type));
  std::replace(result.begin(), result.end(), '_', ' ');
  result += " " + std::to_string(static_cast<int>(type));
  return result;
}