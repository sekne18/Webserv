/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:33:36 by fmol              #+#    #+#             */
/*   Updated: 2025/04/17 13:12:24 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <map>

#include "ISessionManager.hpp"
#include "ILogger.hpp"
#include "ISession.hpp"

class SessionManager : public ISessionManager
{
public:
	SessionManager(const ILogger &logger);
	~SessionManager();

	void attachSessionCookie(IResponse &response, const std::string &sessionId);
	void cleanExpiredSessions();

private:
	std::map<std::string, ISession *> _sessions; // sessionId -> sessionData
	const ILogger &_logger;
};

#endif // SESSIONMANAGER_HPP