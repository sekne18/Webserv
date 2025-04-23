/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:07:34 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:42:56 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "IDispatcher.hpp"

/**
 * @interface IServer
 * @brief Interface for a server class.
 * @ingroup interfaces
 */
class IServer
{
  public:
    virtual ~IServer() {};

    /**
     * @brief Set the dispatcher for the server.
     * @param dispatcher The dispatcher to set.
     */
    virtual void setDispatcher(IDispatcher *dispatcher) = 0;

    /**
     * @brief specify the IP and port to listen on.
     * @param ip The IP address to listen on.
     * @param port The port to listen on.
     */
    virtual void listenOn(const std::string &ip, size_t port) = 0;

    /**
     * @brief Start the server.
     * @details this method starts the server if setDispatcher() was called.
     * @note This method should block until the server is stopped.
     */
    virtual void run() = 0;
};

#endif // ISERVER_HPP