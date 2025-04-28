/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:45:41 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 13:31:25 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

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

    std::string root; //reroute to this prefix
    std::string locationPath; // incoming path prefix

    std::string index; // index file, auto or off

    std::vector<std::string> allowedMethods;
    size_t disallowedReturnCode; // return code if method is not allowed
    std::string disallowedPath; // path to redirect to if method is not allowed

    bool isCgi;
};

struct ServerData
{
    std::string ip;
    size_t port;
    std::vector<Route> routes;
    std::vector<std::string> serverNames;
    std::map<size_t, std::string *> errorPages;
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
    const std::vector<ServerData> &getServerData() const;
    const std::vector<std::pair<std::string, size_t> > getSocketInfo() const;

  private:
    void loadServerBlock(ConfigBlock const &block);
    void loadLocationBlock(ConfigBlock const &block, ServerData &server);
    void loadAllowBlock(ConfigBlock const &block, Route &route);
    std::vector<ServerData> _serverData;
};

#endif // CONFIG_HPP