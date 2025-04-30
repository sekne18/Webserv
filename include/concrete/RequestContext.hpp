/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:49:31 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 13:19:46 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONTEXT_HPP
#define REQUESTCONTEXT_HPP

#include <string>

#include "IRequestContext.hpp"
#include "ISession.hpp"

class RequestContext : public IRequestContext
{
public:
    RequestContext(size_t port, const std::string &ip, ISession *session);
    ~RequestContext();

	const std::string &getIp() const; // override;
	size_t getPort() const; // override;
	ISession *getSession() const; // override;
private:
	std::string _ip;
	size_t _port;
	ISession *_session;
};

#endif // REQUESTCONTEXT_HPP