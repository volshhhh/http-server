#pragma once

#include <algorithm>
#include <iostream>
#include <magic_enum.hpp>
#include <string>

enum ResponseType : int {
  OK = 200,
  Created = 201,
  Not_Found = 404,
  Internal_Server_Error = 500
};

namespace magic_enum {
template <> struct customize::enum_range<ResponseType> {
  static constexpr int min = 200;
  static constexpr int max = 500;
};
} // namespace magic_enum

inline std::string toString(ResponseType type) {
  std::string result = std::string(magic_enum::enum_name(type));
  std::cout << result << std::endl;
  std::replace(result.begin(), result.end(), '_', ' ');
  std::string ans = std::to_string(static_cast<int>(type)) + " " + result;
  return ans;
}