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
  size_t contentLength = 0;
  bool headersReceived = false;
  size_t bodyStartPos = 0;

  // Read data from the socket
  if (!readDataFromSocket(buffer, sizeof(buffer))) {
    return false; // Error or client disconnected
  }

  // Process headers if not already done
  if (!headersReceived) {
    headersReceived = processHeaders(contentLength, bodyStartPos);
  }

  // Check if the request is complete
  if (headersReceived && isRequestComplete(contentLength, bodyStartPos)) {
    // Parse the request if complete
    parseRequest();
    return true; // Request is complete
  }

  // If we've received headers but not the complete body, return and wait for more data
  if (headersReceived && !isRequestComplete(contentLength, bodyStartPos)) {
    return true; // More data needed
  }
  
  // Still waiting for complete headers
  return true;
}

void Client::parseRequest()
{
  _request = Request(_rawRequest.c_str());
  _hasCompleteRequest = true;
}

bool Client::readDataFromSocket(char *buffer, size_t bufferSize)
{
  ssize_t bytesRead = recv(_socket, buffer, bufferSize, MSG_DONTWAIT);

  if (bytesRead <= 0) {
    // Either an error occured or the client disconnected
    close(_socket);
    return false;
  }

  // Append the received data tp _rawRequest
  _rawRequest.append(buffer, bytesRead);
  return true;
}

bool Client::processHeaders(size_t &bodyStartPos, size_t &contentLength)
{
  size_t headerEndPos = _rawRequest.find("\r\n\r\n");
  if (headerEndPos == std::string::npos) {
    return false; // Headers not fully received
  }

  // Headers are fully received
  bodyStartPos = headerEndPos + 4; // Skip "\r\n\r\n"
  
  // Extract Content-Length
  size_t contentPos = _rawRequest.find("Content-Length: ");
  if (contentPos != std::string::npos) {
    contentPos += 16; // Move past "Content-Type: "
    size_t endPos = _rawRequest.find("\r\n", contentPos);
    if (endPos != std::string::npos) {
      std::string contentLengthStr = _rawRequest.substr(contentPos, endPos - contentPos);
      contentLength = std::strtol(contentLengthStr.c_str(), NULL, 10);
    }
  } else {
    contentLength = 0; // No Content-Length header
  }
  
  return true; // Headers processed
}

bool Client::isRequestComplete(size_t bodyStartPos, size_t contentLength)
{
  if (contentLength == 0) {
    // No body expected, request is complete
    return true;
  }

  // Check if the entire body has been received
  size_t currentBodySize = _rawRequest.size() - bodyStartPos;
  return currentBodySize >= contentLength;
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

