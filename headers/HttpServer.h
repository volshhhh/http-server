#pragma once

#include "RequestType.h"
#include "ResponseFactory.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class HttpServer {
private:
  int server_fd = -1;
  std::optional<std::string> dir;
  bool canHandleFiles = false;
  sockaddr_in server_addr;
  ResponseBuilderFactory factory;

  void cleanup();
  std::string build_response(Response &rsp);

public:
  explicit HttpServer(int port, std::optional<std::string> dir_ = std::nullopt);
  ~HttpServer() { cleanup(); }
  HttpServer(const HttpServer &) = delete;
  HttpServer &operator=(const HttpServer &) = delete;

  int getServerFd();

  void handleClientRequest(int client_fd);
};