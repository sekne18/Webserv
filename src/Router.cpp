/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:31:19 by fmol              #+#    #+#             */
/*   Updated: 2025/04/09 10:44:23 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

Router::Router(const std::vector<ServerData> &servers) : _servers(servers)
{
}

Router::Router(const ServerConfig &serverConfig) : _servers(serverConfig.getServers())
{
}

Router::~Router()
{
}

Router::Router(Router const &src)
{
	*this = src;
}

Router &Router::operator=(Router const &rhs)
{
	if (this != &rhs)
	{
	}
	return *this;
}