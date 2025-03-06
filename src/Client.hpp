#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include "Request.hpp"

class Client {
private:
  int _socket;
  sockaddr_in _address;
  std::string _rawRequest;
  Request _request;
  bool _hasCompleteRequest;
  bool processHeaders(size_t &bodyStartPos, size_t &contentLength);
  bool readDataFromSocket(char *buffer, size_t bufferSize);
  void parseRequest();
  bool isRequestComplete(size_t bodyStartPos, size_t contentLength);


public:
  Client();
  Client(int socket, sockaddr_in address);
  ~Client();
  
  bool readRequest();
  int getSocket() const;
  bool hasCompleteRequest() const;
  Request getRequest() const;
};

#endif // CLIENT_HPP
