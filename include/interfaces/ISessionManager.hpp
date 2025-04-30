/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISessionManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:23:32 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:58:18 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISESSIONMANAGER_HPP
#define ISESSIONMANAGER_HPP

#include <string>

#include "ISession.hpp"

class ISessionManager
{
public:
	virtual ~ISessionManager() {};
	
	virtual std::string createSession(size_t expirationTime = 3600) = 0;
	virtual ISession *getSession(const std::string &sessionId) = 0;
	virtual void deleteSession(const std::string &sessionId) = 0;
	virtual void cleanExpiredSessions() = 0;
	virtual bool isSessionExpired(const std::string &sessionId) = 0;
};

#endif // ISESSIONMANAGER_HPP