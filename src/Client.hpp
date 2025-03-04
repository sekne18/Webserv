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

public:
  Client();
  Client(int socket, sockaddr_in address);
  ~Client();
  
  bool readRequest();  // Returns false if client disconnected
  int getSocket() const;
  bool hasCompleteRequest() const;
  Request getRequest() const;
};

#endif // CLIENT_HPP
