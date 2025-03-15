/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:29:58 by fmol              #+#    #+#             */
/*   Updated: 2025/03/12 16:33:53 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "ServerConfig.hpp"

class Router
{
public:
	Router(const std::vector<Server> &servers);
	~Router();
	Router(Router const &src);

private:
	Router &operator=(Router const &rhs);
	const std::vector<Server> _servers;
};

#endif // ROUTER_HPP