#include <iostream>
#include <string>
#include "Server.hpp"
#include "Config.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "ServerConfig.hpp"

void displayUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " [config_file]" << std::endl;
    std::cerr << "  config_file: Path to server configuration file (default: config/default.conf)" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string configFile = "config/default.conf";

    if (argc > 1)
    {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help" || argc > 2)
        {
            displayUsage(argv[0]);
            return (0);
        }
        configFile = argv[1];
    }  
    try
    {
        ServerConfig serverConfig;
        {
            std::ifstream file(configFile);
            if (!file.is_open())
            {
                std::cerr << "Failed to open configuration file: " << configFile << std::endl;
                return (1);
            }
            Lexer lexer(file);
            Parser parser(lexer);
            ConfigBlock config = parser.parse();
            serverConfig.loadConfig(config);
        }
        Server server(serverConfig);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }
    return (0);
}
