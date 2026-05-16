#include <iostream>
#include <exception>
#include "Client.h"

#define EXPECTED_ARGS 2

int main(int argc, char* argv[]) {


    if (argc != EXPECTED_ARGS + 1) {
        std::cerr << "Bad program call. The following call is expected:"
                  << "./client <hostname> <servname>" << std::endl;
        return 1;
    }

    try {
        Client client(argv[1], argv[2]);
        client.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}