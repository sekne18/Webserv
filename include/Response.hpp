#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sys/stat.h>
#include <unistd.h>
#include "Config.hpp"
#include "Request.hpp"

class Response {
private:
  const Config& _config;
  
  std::string getMimeType(const std::string& filePath);

public:
  Response(const Config& config);
  
  void processRequest(const Request& request, int clientSocket);
  void serveStaticFile(const std::string& filePath, int clientSocket);
  void handleFileUpload(const std::string& body, int clientSocket);
  void sendErrorResponse(int statusCode, const std::string& statusMessage, int clientSocket);
  void handleDeleteResponse(const std::string& filePath, int clientSocket);
};

#endif // RESPONSE_HPP
