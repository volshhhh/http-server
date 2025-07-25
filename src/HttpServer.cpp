#include "HttpServer.h"
#include "Request.h"

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

std::string HttpServer::build_response(Response &rsp) {
  std::string status_text;
  switch (rsp.type) {
  case 200:
    status_text = "OK";
    break;
  case 404:
    status_text = "Not Found";
    break;
  default:
    status_text = "Unknown";
    break;
  }

  std::string response = "HTTP/1.1 " + std::to_string(rsp.type) + " " +
                         status_text + "\r\n" +
                         "Content-Type: " + rsp.content_type + "\r\n" +
                         "Content-Length: " + std::to_string(rsp.body.size()) +
                         "\r\n" + "Connection: close\r\n" + "\r\n" + rsp.body;

  return response;
}

void HttpServer::handleClientRequest(int client_fd) {
  char buffer[4096];
  int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

  if (bytes_received < 0) {
    return;
  }
  std::cerr << "Client connected\n";

  std::string request(buffer);

  Request req(request, canHandleFiles);
  Response rsp = factory.getResponse(req, dir);

  std::string response = build_response(rsp);

  send(client_fd, response.c_str(), strlen(response.c_str()), 0);

  close(client_fd);
}
