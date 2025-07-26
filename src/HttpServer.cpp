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

std::string HttpServer::build_response(Response &rsp) {
  std::string status_text;
  switch (rsp.type) {
  case 200:
    status_text = "OK";
    break;
  case 201:
    status_text = "Created";
    break;
  case 404:
    status_text = "Not Found";
    break;
  case 500:
    status_text = "Internal Error";
    break;
  default:
    status_text = "Unknown";
    break;
  }

  std::string connection = (rsp.keep_alive) ? "keep-alive" : "close";

  std::string response =
      "HTTP/1.1 " + std::to_string(rsp.type) + " " + status_text + "\r\n" +
      "Content-Type: " + rsp.content_type + "\r\n" +
      "Content-Length: " + std::to_string(rsp.body.size()) + "\r\n" +
      "Connection: " + connection + "\r\n" + "\r\n" + rsp.body;

  return response;
}

std::vector<std::string> parse_http_requests(const char *buffer,
                                             int bytes_received) {
  std::vector<std::string> requests;
  std::string raw_data(buffer, bytes_received);

  size_t pos = 0;
  while (pos < raw_data.length()) {
    size_t headers_end = raw_data.find("\r\n\r\n", pos);
    if (headers_end == std::string::npos) {
      break;
    }

    size_t content_length = 0;
    size_t cl_pos = raw_data.find("Content-Length: ", pos);
    if (cl_pos != std::string::npos && cl_pos < headers_end) {
      cl_pos += 16;
      size_t cl_end = raw_data.find("\r\n", cl_pos);
      content_length = std::stoul(raw_data.substr(cl_pos, cl_end - cl_pos));
    }

    size_t request_end = headers_end + 4 + content_length;
    if (request_end > raw_data.length()) {
      break;
    }

    requests.push_back(raw_data.substr(pos, request_end - pos));
    pos = request_end;
  }

  return requests;
}

void HttpServer::handleClientRequest(int client_fd) {
  char buffer[4096];
  bool keep_alive = true;

  while (keep_alive) {
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

    keep_alive =
        !(req.requestHeaders.find("Connection") != req.requestHeaders.end() &&
          req.requestHeaders.at("Connection") == "close");

    rsp.keep_alive = keep_alive;

    std::string response = build_response(rsp);
    send(client_fd, response.c_str(), response.size(), 0);

    if (!keep_alive) {
      break;
    }
  }

  close(client_fd);
}