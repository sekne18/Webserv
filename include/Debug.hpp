#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "Lexer.hpp"
#include "Parser.hpp"
#include "ServerConfig.hpp"
#include <iomanip>

void printToken(Token &token);
void printConfigBlock(const ConfigBlock &block, std::string prefix = "", bool hasSibling = false);
void printRoute(const Route &route, int indentLevel);
void printServer(const ServerData &server);
void printServers(const std::vector<ServerData> &servers);

#endif // DEBUG_HPP
