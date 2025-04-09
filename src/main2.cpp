#include "Lexer.hpp"
#include "Parser.hpp"
#include "ServerConfig.hpp"
#include <iomanip>

std::string getType(TokenType::Type type)
{
	switch (type)
	{
	case TokenType::SERVER:
		return "SERVER";
	case TokenType::LOCATION:
		return "LOCATION";
	case TokenType::ROOT:
		return "ROOT";
	case TokenType::LISTEN:
		return "LISTEN";
	case TokenType::SERVER_NAME:
		return "SERVER_NAME";
	case TokenType::INDEX:
		return "INDEX";
	case TokenType::ERROR_PAGE:
		return "ERROR_PAGE";
	case TokenType::MAX_SIZE:
		return "MAX_SIZE";
	case TokenType::L_BRACE:
		return "L_BRACE";
	case TokenType::R_BRACE:
		return "R_BRACE";
	case TokenType::COLON:
		return "COLON";
	case TokenType::SEMICOLON:
		return "SEMICOLON";
	case TokenType::DOMAIN_VALUE:
		return "DOMAIN_VALUE";
	case TokenType::PATH:
		return "PATH";
	case TokenType::IP_VALUE:
		return "IP_VALUE";
	case TokenType::NUMBER:
		return "NUMBER";
	case TokenType::END:
		return "END";
	case TokenType::ALLOW:
		return "ALLOW";
	case TokenType::RETURN:
		return "RETURN";
	case TokenType::REQUEST:
		return "REQUEST";
	case TokenType::AUTO:
		return "AUTO";
	case TokenType::OFF:
		return "OFF";
	default:
		return "UNKNOWN";
	}
}

void printToken(Token &token)
{
	const int width = 20;
	std::cout << std::left << std::setw(width) << token.value;
	std::cout << std::setw(width) << getType(token.type) << "\n";
}

void printConfigBlock(const ConfigBlock& block, std::string prefix = "", bool hasSibling = false)
{
	std::cout << prefix << "+-- Block: " << block.name << "\n";
	prefix += "|   ";
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = block.directives.begin(); it != block.directives.end(); ++it)
	{
		std::cout << prefix << it->first << " =";
		for (std::vector<std::string>::const_iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
		{
			std::cout << " " << *vecIt;
		}
		std::cout << "\n";
	}
	
	for (std::vector<ConfigBlock>::const_iterator childIt = block.children.begin(); childIt != block.children.end(); ++childIt)
	{
		printConfigBlock(*childIt, prefix, childIt + 1 != block.children.end());
	}
	if (!hasSibling)
		std::cout << prefix.substr(0, prefix.size() - 4) << "+--------\n";
}

void printRoute(const Route& route, int indentLevel)
{
    std::string indent(indentLevel, '\t');
    std::cout << indent << "Route:\n";
    std::cout << indent << "  Location Path: " << route.locationPath << "\n";
    std::cout << indent << "  Root: " << route.root << "\n";
    std::cout << indent << "  Index: " << route.index << "\n";
    std::cout << indent << "  Is Return: " << (route.isReturn ? "Yes" : "No") << "\n";
    if (route.isReturn)
    {
        std::cout << indent << "    Return Code: " << route.returnCode << "\n";
        std::cout << indent << "    Return Path: " << route.returnPath << "\n";
    }
    std::cout << indent << "  Allowed Methods: ";
    for (size_t i = 0; i < route.allowedMethods.size(); ++i)
    {
        std::cout << route.allowedMethods[i] << (i + 1 < route.allowedMethods.size() ? ", " : "\n");
    }
    std::cout << indent << "  Disallowed Return Code: " << route.disallowedReturnCode << "\n";
    std::cout << indent << "  Disallowed Path: " << route.disallowedPath << "\n";
    std::cout << indent << "  Error Pages:\n";
	if (route.errorPages.empty())
		std::cout << indent << "    Default\n";
	else
	{
		for (std::map<size_t, std::string>::const_iterator it = route.errorPages.begin(); it != route.errorPages.end(); ++it)
		{
			std::cout << indent << "    " << it->first << " -> " << it->second << "\n";
		}
	}
    std::cout << indent << "  CGI Enabled: " << (route.isCGI ? "Yes" : "No") << "\n";
}

void printServer(const Server& server)
{
    std::cout << "\n=====================================\n";
    std::cout << "Server: " << server.ip << ":" << server.port << "\n";
    std::cout << "=====================================\n";
    std::cout << "Server Names: ";
	if (server.serverNames.empty())
		std::cout << "None\n";
	else
	{
		for (size_t i = 0; i < server.serverNames.size(); ++i)
		{
			std::cout << server.serverNames[i] << (i + 1 < server.serverNames.size() ? ", " : "\n");
		}
	}
    std::cout << "Default Root: " << server.defaultRoot << "\n";
    std::cout << "Default Index: " << server.defaultIndex << "\n";
    std::cout << "Max Size: " << server.max_size << "\n";
    std::cout << "Error Pages:\n";
	if (server.errorPages.empty())
		std::cout << "  Default\n";
	else
	{
		for (std::map<size_t, std::string>::const_iterator it = server.errorPages.begin(); it != server.errorPages.end(); ++it)
		{
			std::cout << "  " << it->first << " -> " << it->second << "\n";
		}
	}
    std::cout << "Routes:\n";
	if (server.routes.empty())
		std::cout << "  Default\n";
	else
	{
		for (size_t i = 0; i < server.routes.size(); ++i)
		{
			printRoute(server.routes[i], 1);
		}
	}
}

void printServers(const std::vector<Server>& servers)
{
    for (size_t i = 0; i < servers.size(); ++i)
    {
        printServer(servers[i]);
        if (i + 1 < servers.size())
            std::cout << "\n-------------------------------------\n";
    }
}


int main()
{
	std::ifstream file("server.conf");
	if (!file.is_open())
	{
		std::cerr << "Failed to open file" << std::endl;
		return (1);
	}
	Lexer lexer(file);
	Parser parser(lexer);
	ServerConfig serverConfig;
	/*
	Token token;
	const int width = 20;
	std::cout << std::left << std::setw(width) << "Token";
	std::cout << std::setw(width) << "Type" << "\n";
	std::cout << std::string(width * 2, '-') << "\n";
	try
	{
		while ((token = lexer.getNextToken()).type != TokenType::END)
		{
			std::cout << std::setw(width) << token.value;
			std::cout << std::setw(width) << getType(token.type) << "\n";
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	*/
	try
	{
		ConfigBlock config = parser.parse();
		std::cout << "done\n\n";
		printConfigBlock(config);
		serverConfig.loadConfig(config);
		const std::vector<Server> &servers = serverConfig.getServers();
		std::cout << "done\n";
		printServers(servers);

	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (0);
}