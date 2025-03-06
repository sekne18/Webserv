#include "Client.hpp"
#include <stdlib.h>

/*
 * Manages client connections and request buffering
 * */
Client::Client() : _socket(-1), _hasCompleteRequest(false)
{
  memset(&_address, 0, sizeof(_address));
}

Client::Client(int socket, sockaddr_in address) 
  : _socket(socket), 
    _address(address),
    _hasCompleteRequest(false)
{
}

Client::~Client()
{
  if (_socket != -1) {
    close(_socket);
    _socket = -1;
  }
}

bool Client::readRequest()
{
  char buffer[4096];
  ssize_t bytesRead;
  size_t contentLength = 0;
  bool headersReceived = false;
  size_t bodyStartPos = 0;

  while (true) {
    bytesRead = recv(_socket, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (bytesRead <= 0) {
      // Either an error occured or client disconeected
      close(_socket);
      return false;
    }

    // Append the received data to _rawRequest
    _rawRequest.append(buffer, bytesRead);

    // Proces headers
    if (!headersReceived) {
      size_t headerEndPos = _rawRequest.find("\r\n\r\n");
      if (headerEndPos != std::string::npos) {
        headersReceived = true;
        bodyStartPos = headerEndPos + 4; // Skip "\r\n\r\n"

        // Extract Content-Length
        size_t contentPos = _rawRequest.find("Content-Length: ");
        if (contentPos != std::string::npos) {
          contentPos += 16; // Move past "Content-Length: "
          size_t endPos = _rawRequest.find("\r\n", contentPos);
          if (endPos != std::string::npos) {
            std::string contentLengthStr = _rawRequest.substr(contentPos, endPos - contentPos);
            contentLength = std::strtol(contentLengthStr.c_str(), NULL, 10);
          }
        } else {
          contentLength = 0;
        }
      }
    }
 
    // Check if we have the complete request
    if (headersReceived) {
      if (contentLength == 0)
        break;
      
      // Validate that the entire body is received
      size_t currentBodySize = _rawRequest.size() - bodyStartPos;
      if (currentBodySize >= contentLength) {
        break;
      }
    }

    // Check if we received all available data
    if (bytesRead < (ssize_t)sizeof(buffer)) {
      // Received less than buffer size, likely all data available for now.
      break;
    }
  }

  // If we've received headers but not the complete body, return and wait for more data
  if (headersReceived && _rawRequest.size() - bodyStartPos < contentLength) {
    return true; // More data needed
  }

  // Parse request if complete
  if (headersReceived && (_rawRequest.size() - bodyStartPos >= contentLength)) {
    _request = Request(_rawRequest.c_str());
    _hasCompleteRequest = true;
    return true;
  }

  // Still waiting for complete headers
  return true;
}

int Client::getSocket() const
{
  return _socket;
}

bool Client::hasCompleteRequest() const
{
  return _hasCompleteRequest;
}

Request Client::getRequest() const
{
  return _request;
}

