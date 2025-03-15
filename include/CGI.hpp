#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
#include "Config.hpp"

class CGI {
private:
  const Config& _config;
  
  void setupChildProcess(int pipefd[2], const std::string& scriptPath, const std::string& queryString);
  void handleParentProcess(int pipefd[2], pid_t pid, int clientSocket);
  std::string readFromPipe(int pipefd);
  void sendCgiResponse(const std::string& cgiOutput, int clientSocket);
  void sendErrorResponse(int statusCode, const std::string& statusMessage, int clientSocket);

public:
  CGI(const Config& config);
  void executeScript(const std::string& scriptPath, const std::string& queryString, int clientSocket);
};

#endif // CGI_HPP
