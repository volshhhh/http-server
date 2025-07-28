#include "HttpServer.h"

void HttpServer::cleanup() {
  if (server_fd != -1) {
    close(server_fd);
    server_fd = -1;
  }
}

HttpServer::HttpServer(int port, std::optional<std::string> dir_) {
  dir = dir_;
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    throw std::runtime_error("Failed to create socket: " +
                             std::string(strerror(errno)));
  }

  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    throw std::runtime_error("setsockopt failed: " +
                             std::string(strerror(errno)));
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    throw std::runtime_error("Failed to bind: " + std::string(strerror(errno)));
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    throw std::runtime_error("Failed to listen: " +
                             std::string(strerror(errno)));
  }

  canHandleFiles = dir.has_value();
  factory = ResponseBuilderFactory(dir);
}

int HttpServer::getServerFd() { return HttpServer::server_fd; }

void HttpServer::handleClientRequest(int client_fd) {
  char buffer[4096];
  bool keepAlive = true;

  while (keepAlive) {
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      if (bytes_received == 0) {
        std::cerr << "Client closed connection\n";
      } else {
        std::cerr << "Receive error: " << strerror(errno) << "\n";
      }
      break;
    }

    std::string httpReq(buffer);
    Request req(httpReq, canHandleFiles);
    Response rsp = factory.getResponse(req, dir);

    keepAlive = rsp.keepAlive();
    std::string response = rsp.to_string();
    send(client_fd, response.c_str(), response.size(), 0);

    if (!keepAlive) {
      break;
    }
  }

  close(client_fd);
}