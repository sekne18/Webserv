/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:50:31 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 13:19:56 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestContext.hpp"

RequestContext::RequestContext(size_t port, const std::string &ip, ISession *session)
	: _ip(ip), _port(port), _session(session)
{
}

RequestContext::~RequestContext()
{
}

const std::string &RequestContext::getIp() const
{
	return _ip;
}

size_t RequestContext::getPort() const
{
	return _port;
}

ISession *RequestContext::getSession() const
{
	return _session;
}