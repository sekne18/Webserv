#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "NetworkManager.hpp"
#include "Config.hpp"
#include "Client.hpp"

class Server
{
private:
  int _port;
  int _serverSocket;
  int _epollFd;
  bool _isRunning;
  struct sockaddr_in _serverAddress;
  std::vector<struct epoll_event> _events;
  std::map<int, Client*> _clients;  // Map of client socket to Client object
  Config _config;
  NetworkManager _networkManager;

  void acceptClient();
  void removeClient(Client *client);
  void processClientEvent(int clientSocket);
  void handleEvents();

public:
  Server(const Config& config);
  ~Server();

  void start();
  void stop();
};

#endif // SERVER_HPP
