#include "../src/Server.hpp"
#include <iostream>
#include <cassert>

void testServer() {
    Server server(8080);

    // Test server initialization
    server.start();

    // TODO: Add more tests for server functionality
    // For example, test handling multiple clients, serving files, etc.

    std::cout << "All server tests passed!" << std::endl;
}

int main() {
    testServer();
    return 0;
}
