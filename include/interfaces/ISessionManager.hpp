/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISessionManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:23:32 by fmol              #+#    #+#             */
/*   Updated: 2025/04/12 15:32:54 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISESSIONMANAGER_HPP
#define ISESSIONMANAGER_HPP

#include <string>

#include "IResponse.hpp"

class ISessionManager
{
public:
	virtual ~ISessionManager() {};

	
	virtual void attachSessionCookie(IResponse &response, const std::string &sessionId) = 0;
	virtual void cleanExpiredSessions() = 0;
};

#endif // ISESSIONMANAGER_HPP