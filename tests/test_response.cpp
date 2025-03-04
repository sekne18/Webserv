#include "../src/Response.hpp"
#include <iostream>
#include <cassert>

void testResponse() {
    Response response;

    // Test setting and getting the status code
    response.setStatusCode(200);
    assert(response.getStatusCode() == 200);

    // Test setting and getting headers
    response.setHeader("Content-Type", "text/html");
    assert(response.getHeader("Content-Type") == "text/html");

    // Test setting and getting the body
    response.setBody("<html><body>Hello, World!</body></html>");
    assert(response.getBody() == "<html><body>Hello, World!</body></html>");

    std::cout << "All response tests passed!" << std::endl;
}

int main() {
    testResponse();
    return 0;
}
