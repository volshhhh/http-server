#pragma once

#include <magic_enum.hpp>
#include <string>

enum RequestType { GET, POST };

inline std::string toString(RequestType type) {
  std::string result = static_cast<std::string>(magic_enum::enum_name(type));
  return result;
}