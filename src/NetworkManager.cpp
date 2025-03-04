#include "NetworkManager.hpp"


/*
 * NetworkManager handles socket and network operations.
 * 1. creates a socket, binds it to a port, listens for incoming connections.
 * 2. sets up an epoll instance to monitor for incoming data.
 * 3. accepts incoming connections and adds them to the epoll set.
 * 4. provides methods to close the socket and epoll instance.
 * */
NetworkManager::NetworkManager(int port) : _port(port)
{
}

int NetworkManager::createSocket()
{
  // Socket is used to listen for incoming connections.
  // AF_INET - IPv4, SOCK_STREAM - TCP
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
    throw std::runtime_error("Error: Failed to create socket. " 
                            + std::string(strerror(errno)));
  std::cout << "Socket created successfully." << "\n";
  return serverSocket;
}

void NetworkManager::bindSocket(int serverSocket, sockaddr_in& serverAddress)
{
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // htons() converts the port number to network byte order
  serverAddress.sin_port = htons(_port);
  
  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
  {
    close(serverSocket);
    throw std::runtime_error("Error: Failed to bind socket. " 
                            + std::string(strerror(errno)));
  }
  std::cout << "Socket bound to port " << _port << "\n";
}

/*
 * Listen for incoming connections on the server socket.
 * listen function listens to serverSocket and allows up to 10 pending connections.
 * */
void NetworkManager::listenForConnections(int serverSocket)
{
  if (listen(serverSocket, 10) == -1)
  {
    close(serverSocket);
    throw std::runtime_error("Error: Failed to listen to socket. " 
                            + std::string(strerror(errno)));
  }
  std::cout << "Server is listening on port " << _port << "..." << "\n";
}

int NetworkManager::setupEpoll(int serverSocket)
{
  // Creates an epoll instance
  int epollFd = epoll_create1(0);
  if (epollFd == -1)
    throw std::runtime_error("Error: Failed to create epoll instance. " 
                            + std::string(strerror(errno)));
  
  // Add the server socket to the epoll set
  struct epoll_event event;
  event.events = EPOLLIN; // Monitor for incoming data
  event.data.fd = serverSocket;

  // epoll_ctl() Add the server socket to the epoll set of monitor for incoming connections (EPOLLIN)
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1)
  {
    close(epollFd);
    throw std::runtime_error("Error: Failed to add server socket to epoll. " 
                            + std::string(strerror(errno)));
  }
  std::cout << "Epoll set up successfully." << "\n";
  return epollFd;
}

Client *NetworkManager::acceptConnection(int serverSocket, int epollFd)
{
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);

  int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
  if (clientSocket == -1)
  {
    throw std::runtime_error("Error: Failed to accept client connection. " 
                            + std::string(strerror(errno)));
  }

  // Set the client socket to non-blocking mode
  int flags = fcntl(clientSocket, F_GETFL, 0);
  fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
  
  // Add the client socket to the epoll set
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET; // Monitor for incoming data (edge-triggered)
  event.data.fd = clientSocket;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
  {
    close(clientSocket);
    throw std::runtime_error("Error: Failed to add client socket to epoll. " 
                            + std::string(strerror(errno)));
  }
  std::cout << "Client connected from " << inet_ntoa(clientAddress.sin_addr) 
            << ":" << ntohs(clientAddress.sin_port) << "\n";
            
  // Create and return a Client object
  return new Client(clientSocket, clientAddress);
}

void NetworkManager::closeSocket(int& socket)
{
  if (socket != -1)
  {
    close(socket);
    socket = -1;
    std::cout << "Socket closed." << "\n";
  }
}

void NetworkManager::closeEpoll(int& epollFd)
{
  if (epollFd != -1)
  {
    close(epollFd);
    epollFd = -1;
    std::cout << "Epoll instance closed." << "\n";
  }
}
