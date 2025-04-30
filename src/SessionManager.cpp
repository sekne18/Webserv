/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:10:53 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 15:42:32 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SessionManager.hpp"

SessionManager::SessionManager(const ILogger &logger, time_t refreshTime)
	: _logger(logger), _refreshTime(refreshTime)
{
}

SessionManager::~SessionManager()
{
	for (std::map<std::string, ISession *>::iterator it = _sessions.begin(); it != _sessions.end(); ++it)
	{
		delete it->second;
	}
}

std::string SessionManager::createSession(size_t expirationTime)
{
	time_t now = time(NULL);
	std::string sessionId = _sessionIdGenerator.generateUniqueSessionId(_sessionIds);
	if (sessionId.empty())
	{
		_logger.logError("Failed to generate unique session ID");
		return "";
	}
	_sessionIds.insert(sessionId);
	_sessions[sessionId] = new Session(sessionId); 
	_expirationTimes[sessionId] = now + expirationTime;
	return sessionId;
}

ISession *SessionManager::getSession(const std::string &sessionId)
{
	std::map<std::string, ISession *>::const_iterator it = _sessions.find(sessionId);
	if (it != _sessions.end())
	{
		time_t now = time(NULL);
		_expirationTimes[sessionId] = now + _refreshTime;
		return it->second;
	}
	return NULL;
}

void SessionManager::deleteSession(const std::string &sessionId)
{
	std::map<std::string, ISession *>::iterator it = _sessions.find(sessionId);
	if (it != _sessions.end())
	{
		delete it->second;
		_sessions.erase(it);
		_sessionIds.erase(sessionId);
		_expirationTimes.erase(sessionId);
	}
}

void SessionManager::cleanExpiredSessions()
{
	time_t now = time(NULL);
	for (std::map<std::string, ISession *>::iterator it = _sessions.begin(); it != _sessions.end();)
	{
		if (now > _expirationTimes[it->first])
		{
			std::map<std::string, ISession *>::iterator tmp = it;
			++tmp;
			delete it->second;
			_sessionIds.erase(it->first);
			_expirationTimes.erase(it->first);
			_sessions.erase(it);
			it = tmp;
		}
		else
			++it;
	}
}

bool SessionManager::isSessionExpired(const std::string &sessionId)
{
	ISession *session = getSession(sessionId);
	if (session)
	{
		time_t now = time(NULL);
		if (now > _expirationTimes[sessionId])
		{
			deleteSession(sessionId);
			return true;
		}
		return false;
	}
	return true;
}