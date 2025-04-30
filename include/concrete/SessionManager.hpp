/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:33:36 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:59:43 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <map>
#include <set>

#include "ISessionManager.hpp"
#include "ILogger.hpp"
#include "Session.hpp"
#include "SessionIdGenerator.hpp"

class SessionManager : public ISessionManager
{
public:
	SessionManager(const ILogger &logger, time_t refreshTime = 3600);
	~SessionManager();

	std::string createSession(size_t expirationTime = 3600); // override;
	ISession *getSession(const std::string &sessionId); // override;
	void deleteSession(const std::string &sessionId); // override;
	void cleanExpiredSessions(); // override;
	bool isSessionExpired(const std::string &sessionId); // override;

private:
	const ILogger &_logger;
	time_t _refreshTime;
	SessionIdGenerator _sessionIdGenerator;
	std::set<std::string> _sessionIds;
	std::map<std::string, ISession *> _sessions; // sessionId -> sessionData
	std::map<std::string, time_t> _expirationTimes; // sessionId -> expirationTime
};

#endif // SESSIONMANAGER_HPP