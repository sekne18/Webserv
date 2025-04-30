/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:37:09 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 13:26:07 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const ILogger &logger)
    : _sessionManager(new SessionManager(logger)),
      _networkManager(new NetworkManager(logger, *_sessionManager)),
      _dispatcher(0),
      _logger(logger)
{
}

Server::~Server()
{
    delete _networkManager;
    delete _sessionManager;
}

void Server::setDispatcher(IDispatcher *dispatcher)
{
    _dispatcher = dispatcher;
    _networkManager->setDispatcher(dispatcher);
}

void Server::listenOn(const std::string &ip, size_t port)
{
    _networkManager->listenOn(ip, port);
}

void Server::run()
{
    _networkManager->run();
}

void Server::stop()
{
    _networkManager->stop();
}