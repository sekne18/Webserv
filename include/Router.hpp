/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:29:58 by fmol              #+#    #+#             */
/*   Updated: 2025/04/09 11:38:59 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "ServerConfig.hpp"
#include "Request.hpp"

class Router
{
public:
	Router(const std::vector<ServerData> &servers);
	Router(const ServerConfig &serverConfig);
	~Router();
	Router(Router const &src);
	const ServerData &route(const Request &request) const;
private:
	Router &operator=(Router const &rhs);
	const std::vector<ServerData> &_servers;
};

#endif // ROUTER_HPP