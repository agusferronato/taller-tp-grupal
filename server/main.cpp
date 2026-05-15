#include <exception>
#include <iostream>

#include "Server.h"

#define EXPECTED_ARGS 1

int main(int argc, char* argv[]) {


    if (argc != EXPECTED_ARGS + 1) {
        std::cerr << "Bad program call. The following call is expected:"
                  << "./server <port>" << std::endl;
        return 1;
    }

    try {
        Server server(argv[1]);
        server.run();
    }

    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}