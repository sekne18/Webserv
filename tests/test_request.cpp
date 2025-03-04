#include "../src/Request.hpp"
#include <iostream>
#include <cassert>

void testRequestParsing() {
    // Test a simple HTTP GET request
    std::string rawRequest =
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "\r\n";

  
    Request request(rawRequest);

    // Test parsing the method
    assert(request.getMethod() == "GET");

    // Test parsing the URL
    assert(request.getUrl() == "/index.html");
 
    // Test parsing headers
    assert(request.getHeader("Host") == "localhost:8080");
    assert(request.getHeader("User-Agent") == "curl/7.68.0");

    // Test a non-existent header
    assert(request.getHeader("Connection") == "");

    std::cout << "All request parsing tests passed!" << std::endl;
}

int main() {
    testRequestParsing();
    return 0;
}


