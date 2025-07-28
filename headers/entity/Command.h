#pragma once

#include "RequestType.h"
#include <algorithm>
#include <magic_enum.hpp>
#include <string>

enum Command { EMPTY, ECHO, USER_AGENT, GET_FILES, POST_FILES, FAIL };

inline Command convertToCommand(RequestType type, std::string &cmd) {
  if (cmd.empty()) {
    return Command::EMPTY;
  }

  auto transform = [](std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](char c) { return (c == '-') ? '_' : toupper(c); });
    return input;
  };

  std::string CMD = transform(cmd);

  auto tmp1 = magic_enum::enum_cast<Command>(CMD);
  if (tmp1.has_value()) {
    return tmp1.value();
  }

  CMD.insert(0, toString(type) + '_');
  auto tmp2 = magic_enum::enum_cast<Command>(CMD);
  if (tmp2.has_value()) {
    return tmp2.value();
  }

  return Command::FAIL;
}