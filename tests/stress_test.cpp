#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/*
 * Purpose of this test:
 * Simulate real-world scenario where multiple clients are sending requests to the server.
 * The server should be able to handle multiple requests concurrently.
 * This test sends 1000 requests to the server with 10 concurrent connections.
 * */

void sendRequest(const std::string& host, int port, const std::string& request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        return;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error: Failed to connect to server." << std::endl;
        close(sock);
        return;
    }

    if (write(sock, request.c_str(), request.size()) == -1) {
        std::cerr << "Error: Failed to send request." << std::endl;
    }

    close(sock);
}

int main() {
    std::string host = "127.0.0.1";
    int port = 8080;
    std::string request =
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "\r\n";

    // Number of requests to send
    int numRequests = 1000;

    // Number of concurrent connections
    int numConcurrent = 10;

    for (int i = 0; i < numRequests; ++i) {
        if (i % numConcurrent == 0) {
            // Simulate concurrent connections
            for (int j = 0; j < numConcurrent; ++j) {
                sendRequest(host, port, request);
            }
        }
        std::cout << "Request " << i + 1 << " sent." << std::endl;
    }

    return 0;
}
