/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:52:10 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 08:27:15 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <queue>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#include "IConnection.hpp"
#include "IDispatcher.hpp"
#include "ILogger.hpp"
#include "IResponse.hpp"
#include "ConcreteResponses.hpp"
#include "RequestParser.hpp"
#include "ResponseWriter.hpp"
#include "Utils.hpp"
#include "structs.hpp"
#include "RequestContext.hpp"

class Connection : public IConnection
{
  public:
    Connection(int epFd, t_socketInfo info, IDispatcher &dispatcher, const ILogger &logger);
    ~Connection();

    void onReadable(); // override;
    void onWritable(); // override;
    bool shouldClose(); // override;
  private:
    void modifyEpoll(int events);
    void deleteResponseQueue();
    
    int _epFd;                          // not owned
    int _socket;                            // owned
    std::string _ip;
    size_t _listenPort;
    size_t _port;
    bool _shouldClose;
    IRequestParser *_RequestParser;         // owned
    IResponseWriter *_responseWriter;       // owned
    std::queue<IResponse *> _responseQueue; // owned
    IDispatcher &_dispatcher;               // not owned
    const ILogger &_logger;                 // not owned
};

#endif // CONNECTION_HPP