/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:45:41 by fmol              #+#    #+#             */
/*   Updated: 2025/04/09 14:05:45 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "Utils.hpp"
#include "Validators.hpp"

/*
* ConfigBlock represents a block in the configuration file.
* although block structure is already validated by the parser,
* actual values are not validated yet (is done when creating a ServerConfig object)
*/
struct ConfigBlock
{
	std::string name;
	std::map<std::string, std::vector<std::string> > directives;
	std::vector<ConfigBlock> children;
};

struct Route
{
	bool isReturn;
	size_t returnCode;
	std::string returnPath;

	std::string root;
	std::string locationPath;

	std::string index;

	std::vector<std::string> allowedMethods;
	size_t disallowedReturnCode;
	std::string disallowedPath;

	std::map<size_t, std::string> errorPages;

	bool isCGI;
};

struct ServerData
{
	std::string ip;
	size_t port;
	std::vector<Route> routes;
	std::vector<std::string> serverNames;
	std::map<size_t, std::string> errorPages;
	std::string defaultRoot;
	std::string defaultIndex;
	size_t max_size;
};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();
	ServerConfig(ServerConfig const &src);
	ServerConfig &operator=(ServerConfig const &rhs);

	void loadConfig(ConfigBlock const &block);
	const std::vector<ServerData> &getServers() const;
private:
	void loadServerBlock(ConfigBlock const &block);
	void loadLocationBlock(ConfigBlock const &block, ServerData &server);
	void loadAllowBlock(ConfigBlock const &block, Route &route);
	std::vector<ServerData> _servers;
};

#endif // CONFIG_HPP