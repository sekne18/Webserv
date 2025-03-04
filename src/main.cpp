#include <iostream>
#include <string>
#include "Server.hpp"
#include "Config.hpp"

void displayUsage(const char* programName) {
  std::cerr << "Usage: " << programName << " [config_file]" << std::endl;
  std::cerr << "  config_file: Path to server configuration file (default: config/default.conf)" << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    std::string configFile = "config/default.conf";
    
    if (argc > 1) {
      if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        displayUsage(argv[0]);
        return 0;
      }
      configFile = argv[1];
    }
    
    std::cout << "Loading configuration from: " << configFile << std::endl;
    Config config(configFile);
    
    std::cout << "Starting server on port " << config.getPort() << std::endl;
    Server server(config);
    server.start(); 
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
