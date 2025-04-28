/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 09:30:24 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 07:52:45 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>

#include "IServer.hpp"
#include "NetworkManager.hpp"
#include "IDispatcher.hpp"
#include "SessionManager.hpp"
#include "ILogger.hpp"

class Server : public IServer
{
public:
	Server(const ILogger &logger);
	~Server();

	void setDispatcher(IDispatcher *dispatcher);	   // override;
	void listenOn(const std::string &ip, size_t port); // override;
	void run();										   // override;
	void stop(); // override;
private:
	INetworkManager *_networkManager; // owned
	ISessionManager *_sessionManager; // owned
	IDispatcher *_dispatcher;		  // not owned
	const ILogger &_logger;			  // not owned
};

#endif // SERVER_HPP