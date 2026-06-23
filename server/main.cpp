#include <exception>
#include <iostream>

#include "Server.h"

#define EXPECTED_ARGS 2

int main(int argc, char *argv[]) {

  if (argc != EXPECTED_ARGS + 1) {
    std::cerr << "Bad program call. The following call is expected:"
              << "./server <port> <mapPath>" << std::endl;
    return 1;
  }

  try {
    Server server(argv[1], argv[2]);
    server.run();
  }

  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
