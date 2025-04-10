#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "Client.hpp"

class NetworkManager
{
private:
  // int _port;

public:
  NetworkManager(int port);

  int createSocket();
  void bindSocket(int serverSocket, sockaddr_in &serverAddress);
  void listenForConnections(int serverSocket);
  int setupEpoll(int serverSocket);
  Client *acceptConnection(int serverSocket, int epollFd);
  void closeSocket(int &socket);
  void closeEpoll(int &epollFd);
};

class NetworkManager
{
public:
  NetworkManager();
  ~NetworkManager();

  void initializeServer(int port, const std::string &address);
  Request getNextRequest();

private:
  NetworkManager(const NetworkManager &other);
  NetworkManager &operator=(const NetworkManager &other);

  int _epollFd;
}
#endif // NETWORKMANAGER_HPP
