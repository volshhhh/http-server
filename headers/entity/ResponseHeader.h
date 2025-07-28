#pragma once

#include <algorithm>
#include <magic_enum.hpp>
#include <string>

enum ResponseHeader {
  Content_Encoding,
  Content_Type,
  Content_Length,
  Connection
};

inline std::string toString(ResponseHeader header) {
  std::string result = static_cast<std::string>(magic_enum::enum_name(header));
  std::replace(result.begin(), result.end(), '_', '-');
  return result;
}
