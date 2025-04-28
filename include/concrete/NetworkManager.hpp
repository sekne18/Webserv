/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:51:03 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 08:32:32 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#include "Connection.hpp"
#include "IDispatcher.hpp"
#include "ILogger.hpp"
#include "INetworkManager.hpp"
#include "IRequestParser.hpp"
#include "Utils.hpp"
#include "assert.hpp"
#include "structs.hpp"

class NetworkManager : public INetworkManager
{
  public:
    NetworkManager(const ILogger &logger, size_t maxEvents = 10);
    ~NetworkManager();

    void run();                                        // override;
    void stop();                                       // override;
    void setDispatcher(IDispatcher *dispatcher);       // override;
    /**
     * * @brief start listening on the given ip and port
     * * @param ip the ip address to listen on
     * * @param port the port to listen on
     * * @throws std::runtime_error if the socket could not be created, bound or set to non-blocking & close on exec
     * * @throws std::runtime_error if the socket could not be registered with epoll
     * * @details will call createSocket and handles an SocketAlreadyInUseException elegantly. any other exception is assumed to be fatal and will be thrown
     * * @note this function will not throw if the socket is already in use, but will log an error message
     */
    void listenOn(const std::string &ip, size_t port); // override;

    class SocketAlreadyInUseException : public std::exception
    {
      public:
        SocketAlreadyInUseException(const std::string &msg);
        virtual const char *what() const throw();
		virtual ~SocketAlreadyInUseException() throw() {};

      private:
        std::string _msg;
    };

  protected:
    int createSocket(const std::string &ip, size_t port);
    void registerSocket(int socket);   // override;
    void unregisterSocket(int socket); // override;

  private:
    volatile bool _shouldStop;
    size_t _maxEvents;
    int _epFd;
    IDispatcher *_dispatcher;
    std::vector<t_socketInfo> _listeners;
    std::map<int, IConnection *> _connections;
    const ILogger &_logger;
};

#endif // NETWORKMANAGER_HPP