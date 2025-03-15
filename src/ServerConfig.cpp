/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:33:45 by fmol              #+#    #+#             */
/*   Updated: 2025/03/12 15:55:06 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(ServerConfig const &src)
{
	*this = src;
}

ServerConfig &ServerConfig::operator=(ServerConfig const &rhs)
{
	if (this != &rhs)
	{
	}
	return *this;
}

void ServerConfig::loadConfig(ConfigBlock const &block)
{
	if (block.name != "main")
		throw std::runtime_error("Root block must be named 'main'");
	for (std::vector<ConfigBlock>::const_iterator it = block.children.begin(); it != block.children.end(); ++it)
		loadServerBlock(*it);
}

void ServerConfig::loadServerBlock(ConfigBlock const &block)
{
	Server server;
	server.max_size = 1000000;
	server.defaultIndex = "auto";
	server.defaultRoot = "/";
	if (block.name != "server")
			throw std::runtime_error("Server block must be named 'server'");
	if (block.directives.find("listen") == block.directives.end())
		throw std::runtime_error("Server block must contain a 'listen' directive");
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = block.directives.begin(); it != block.directives.end(); ++it)
	{
		if (it->first == "listen")
		{
			if (it->second.size() == 1)
			{
				server.ip = "any";
				server.port = toSizeT(it->second[0]);
			}
			else if (it->second.size() == 2)
			{
				server.ip = it->second[0];
				validateIp(server.ip);
				server.port = toSizeT(it->second[1]);
			}
			else
				throw std::runtime_error("Invalid number of arguments for 'listen' directive");
			validatePort(server.port);
		}
		else if (it->first == "server_name")
		{
			for (std::vector<std::string>::const_iterator nameIt = it->second.begin(); nameIt != it->second.end(); ++nameIt)
			{
				validateServerName(*nameIt);
				server.serverNames.push_back(*nameIt);
			}
		}
		else if (it->first == "root")
		{
			validatePath(it->second[0]);
			server.defaultRoot = it->second[0];
		}
		else if (it->first == "max_size")
		{
			validateMaxSize(toSizeT(it->second[0]));
			server.max_size = toSizeT(it->second[0]);
		}
		else if (it->first == "error_page")
		{
			validateReturnCode(toSizeT(it->second[0]));
			validatePath(it->second[1]);
			server.errorPages[toSizeT(it->second[0])] = it->second[1];
		}
		else if (it->first == "index")
		{
			if (it->second[0] != "auto" && it->second[0] != "off")
				validatePath(it->second[0]);
			server.defaultIndex = it->second[0];
		}
		else
			throw std::runtime_error("Invalid directive in server block");
	}
	for (std::vector<ConfigBlock>::const_iterator it = block.children.begin(); it != block.children.end(); ++it)
		loadLocationBlock(*it, server);
	_servers.push_back(server);
}

void ServerConfig::loadLocationBlock(ConfigBlock const &block, Server &server)
{
	Route route;
	route.isCGI = false;
	route.isReturn = false;
	route.index = server.defaultIndex;
	route.root = server.defaultRoot;
	if (block.name != "location")
		throw std::runtime_error("Location block must be named 'location'");
	if (block.directives.size() <= 1)
		throw std::runtime_error("Location block must contain at least one directive");
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = block.directives.begin(); it != block.directives.end(); ++it)
	{
		if (it->first == "path")
		{
			validatePath(it->second[0]);
			route.locationPath = it->second[0];
			if (hasCGIExtension(route.locationPath))
				route.isCGI = true;
		}
		else if (it->first == "root")
		{
			validatePath(it->second[0]);
			route.root = it->second[0];
		}
		else if (it->first == "return")
		{
			route.isReturn = true;
			route.returnCode = toSizeT(it->second[0]);
			validateReturnCode(route.returnCode);
			if (it->second.size() == 2)
			{
				validatePath(it->second[1]);
				route.returnPath = it->second[1];
			}
		}
		else if (it->first == "index")
		{
			if (it->second[0] != "auto" && it->second[0] != "off")
				validatePath(it->second[0]);
			route.index = it->second[0];
		}
		else if (it->first == "error_page")
		{
			validateReturnCode(toSizeT(it->second[0]));
			validatePath(it->second[1]);
			route.errorPages[toSizeT(it->second[0])] = it->second[1];
		}
		else
			throw std::runtime_error("Invalid directive in location block");
	}
	for (std::vector<ConfigBlock>::const_iterator it = block.children.begin(); it != block.children.end(); ++it)
		loadAllowBlock(*it, route);
	if (route.allowedMethods.empty())
	{
		route.allowedMethods = getValidMethods();
		route.disallowedReturnCode = 501;
		route.disallowedPath = "";
	}
	server.routes.push_back(route);
}

void ServerConfig::loadAllowBlock(ConfigBlock const &block, Route &route)
{
	if (block.name != "allow")
		throw std::runtime_error("Allow block must be named 'allow'");
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = block.directives.begin(); it != block.directives.end(); ++it)
	{
		if (it->first == "method")
		{
			for (std::vector<std::string>::const_iterator methodIt = it->second.begin(); methodIt != it->second.end(); ++methodIt)
			{
				validateMethod(*methodIt);
				route.allowedMethods.push_back(*methodIt);
			}
		}
		else if (it->first == "return")
		{
			route.disallowedReturnCode = toSizeT(it->second[0]);
			validateReturnCode(route.disallowedReturnCode);
			if (it->second.size() == 2)
			{
				validatePath(it->second[1]);
				route.disallowedPath = it->second[1];
			}
		}
		else
			throw std::runtime_error("Invalid directive in allow block");
	}
}

const std::vector<Server> &ServerConfig::getServers() const
{
	return _servers;
}