#include "libs.h"

int main(int argc, char **argv) {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::optional<std::string> dir;
  if (argc >= 3 && strcmp(argv[1], "--directory") == 0) {
    dir = argv[2];
  }
  HttpServer server(4221, dir);

  while (true) {
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server.getServerFd(), (sockaddr *)&client_addr,
                           &client_addr_len);

    if (client_fd < 0) {
      std::cerr << "Failed to accept connection\n";
      continue;
    }
    // server.handleClientRequest(client_fd);
    std::thread(&HttpServer::handleClientRequest, &server, client_fd).detach();
  }
}