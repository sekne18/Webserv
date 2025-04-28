/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:07:56 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 09:24:00 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include <iostream>
#include <csignal>

#include "Dispatcher.hpp"
#include "Lexer.hpp"
#include "NetworkManager.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "StreamLogger.hpp"

void printUsage()
{
    std::cout << "Usage: server [options]\n";
    std::cout << "Options:\n";
    std::cout << "  --log-file <file>       Set the log file (default: stdout)\n";
    std::cout << "  --config-file <file>    Set the config file (default: server.conf)\n";
    std::cout << "  --log-level <level>     Set the log level (info, debug, warning, error)\n";
}

IServer *g_server = 0;

void sigIntHandler(int signum)
{
    (void)signum;
    if (g_server)
        g_server->stop();
}

int main(int argc, char **argv)
{
    std::signal(SIGINT, sigIntHandler);
    std::string logFile = "";
    std::string configFile = "server.conf";
    ILogger::LogLevel logLevel = ILogger::LOG_DEBUG | ILogger::LOG_ERROR | ILogger::LOG_WARNING | ILogger::LOG_INFO;
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (std::string(argv[i]) == "--log-file" && i + 1 < argc)
            {
                logFile = argv[++i];
            }
            else if (std::string(argv[i]) == "--config-file" && i + 1 < argc)
            {
                configFile = argv[++i];
            }
            else if (std::string(argv[i]) == "--log-level" && i + 1 < argc)
            {
                std::string level = argv[++i];
                if (level == "info")
                    logLevel = ILogger::LOG_INFO | ILogger::LOG_ERROR | ILogger::LOG_WARNING;
                else if (level == "debug")
                    logLevel = ILogger::LOG_DEBUG | ILogger::LOG_ERROR | ILogger::LOG_WARNING | ILogger::LOG_INFO;
                else if (level == "warning")
                    logLevel = ILogger::LOG_WARNING | ILogger::LOG_ERROR;
                else if (level == "error")
                    logLevel = ILogger::LOG_ERROR;
                else
                    std::cerr << "Unknown log level: " << level << std::endl;
            }
            else
            {
                std::cerr << "Unknown argument / no value for: " << argv[i] << std::endl;
                printUsage();
                return (1);
            }
        }
    }
    std::ifstream file(configFile.c_str());
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return (1);
    }
    if (logFile != "")
    {
        std::ofstream logStream(logFile.c_str());
        if (!logStream.is_open())
        {
            std::cerr << "Failed to open log file" << std::endl;
            return (1);
        }
        StreamLogger::getInstance()->setLogStream(logStream);
    }
    else
        StreamLogger::getInstance()->setLogStream(std::cout);
    StreamLogger::getInstance()->setLogLevel(logLevel);
    try
    {
        g_server = new Server(*StreamLogger::getInstance());
        Dispatcher dispatcher(*StreamLogger::getInstance());
        {
            Lexer lexer(file);
            Parser parser(lexer);
            ServerConfig serverConfig;
            ConfigBlock config = parser.parse();
            serverConfig.loadConfig(config);
            dispatcher.loadFromConfig(serverConfig);
            const std::vector<std::pair<std::string, size_t> > socketInfo = serverConfig.getSocketInfo();
            for (std::vector<std::pair<std::string, size_t> >::const_iterator it = socketInfo.begin(); it != socketInfo.end(); ++it)
                g_server->listenOn(it->first, it->second);
        }
        g_server->setDispatcher(&dispatcher);
        g_server->run();
    }
    catch (std::exception &e)
    {
        StreamLogger::getInstance()->logError(std::string(e.what()));
        delete g_server;
        g_server = 0;
        StreamLogger::destroyInstance();
        return (1);
    
    }
    delete g_server;
    g_server = 0;
    StreamLogger::destroyInstance();
    return (0);
}