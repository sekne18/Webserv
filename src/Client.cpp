#include "Client.hpp"

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
  std::string rawRequest;

  while (true) {
    ssize_t bytesRead = read(_socket, buffer, sizeof(buffer));
    
    if (bytesRead == -1)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // No more data available at the moment
        break;
      }
      else {
        std::cerr << "Error: Failed to read from client socket. " 
                  << strerror(errno) << "\n";
        close(_socket);
        return false;
      }
    } else if (bytesRead == 0) {
      // Client disconnected
      std::cout << "Client disconnected." << "\n";
      close(_socket);
      return false;
    }

    // This will append the buffer to the rawRequest string
    _rawRequest.append(buffer, bytesRead);

    std::cout << "Request " << _rawRequest << "\n";

    // Check if the entire request has been received
    // For multipart form-data, look for the boundary end marker
    size_t boundaryEnd = _rawRequest.find("--\r\n");
    if (boundaryEnd != std::string::npos) {
      // End of request detected
      break;
    }
  }
  
  /*
  std::cout << "=======================================================" << "\n";
  std::cout << "Received raw request: \n" << _rawRequest << "\n";
  std::cout << "=======================================================" << "\n";
  */
  
  // Check if the request is complete
  if (_rawRequest.find("--\r\n") == std::string::npos) {
    return true; // More data might be coming
  }

  // Request is complete, parse it
  _request = Request(_rawRequest.c_str());
  _hasCompleteRequest = true;
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

