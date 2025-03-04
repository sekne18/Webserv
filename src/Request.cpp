#include "Request.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

/*
  * Request class represents an HTTP request.
  * It receives a raw request string (for example "GET /index.html HTTP/1.1")
  * and parses it into the HTTP method, URL, headers, and body.
 */
Request::Request() {}

Request::Request(const std::string &rawRequest)
{
  parseRequest(rawRequest);
}

void Request::parseRequest(const std::string &rawRequest)
{
  std::istringstream requestStream(rawRequest);
  std::string line;

  // Print the raw request for debugging
  //std::cerr << "Raw request: " << rawRequest << "\n";

  // Parse the request line (for example "GET /index.html HTTP/1.1")
  std::getline(requestStream, line);
  std::istringstream requestLineStream(line);
  requestLineStream >> _method >> _url;

  // Parse headers (for example "Host: localhost::8080")
  while (std::getline(requestStream, line) && line != "\r") // Why "\r"?
  {
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos)
    {
      std::string key = line.substr(0, colonPos);
      std::string value = line.substr(colonPos + 2); // Skip ": " after the colon
      _headers[key] = value;
    }
  }

  // Parse the body (if any)
  std::getline(requestStream, _body, '\0');
}

std::string Request::getMethod() const
{
  return _method;
}

std::string Request::getUrl() const
{
  return _url;
}

std::string Request::getHeader(const std::string &key) const
{
  std::map<std::string, std::string>::const_iterator it = _headers.find(key);
  // If the key is not found, find() returns _headers.end(),
  // which is a special iterator representing the "end" of the map
  return (it != _headers.end()) ? it->second : "";
}

std::string Request::getBody() const 
{
  return _body;
}

