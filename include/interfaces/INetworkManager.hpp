/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INetworkManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:12:33 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 07:54:52 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INETWORKMANAGER_HPP
#define INETWORKMANAGER_HPP

#include <string>

#include "IDispatcher.hpp"

/*
 * INetworkManager interface:
 * This interface defines the methods for managing network connections.
 */
class INetworkManager
{
  public:
    virtual ~INetworkManager() {};

    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void listenOn(const std::string &ip, size_t port) = 0;
    virtual void setDispatcher(IDispatcher *dispatcher) = 0;
    /**
     * @note a function to register a timed event which will be called within the epoll loop.
     * implement this later if necessary (WIP)
     */
    // virtual void registerTimedEvent();

  protected:
    virtual void registerSocket(int socket) = 0;
    virtual void unregisterSocket(int socket) = 0;
};

#endif // INETWORKMANAGER_HPP