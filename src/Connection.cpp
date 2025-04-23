/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:30:32 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 16:41:25 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(int epFd, t_socketInfo info, IDispatcher &dispatcher, const ILogger &logger)
    : _epFd(epFd), _socket(info.fd), _ip(info.ip), _port(info.port), _RequestParser(new RequestParser(logger)),
      _dispatcher(dispatcher), _logger(logger)
{
}

Connection::~Connection()
{
    if (_socket != -1)
        close(_socket);
    delete _RequestParser;
    if (_responseQueue.size() > 0)
    {
        while (!_responseQueue.empty())
        {
            IResponse *response = _responseQueue.front();
            _responseQueue.pop();
            delete response;
        }
    }
}

void Connection::modifyEpoll(int events)
{
    struct epoll_event event;
    event.data.fd = _socket;
    event.events = events;
    if (epoll_ctl(_epFd, EPOLL_CTL_MOD, _socket, &event) == -1)
    {
        _logger.logError("Failed to modify epoll event for socket " + toString(_socket) + ": " + strerror(errno));
        throw std::runtime_error("Failed to modify epoll event");
    }
}

void Connection::onReadable()
{
    std::vector<char> buffer(4096, 0);
    while (true)
    {
        ssize_t bytesRead = recv(_socket, buffer.data(), 4096, MSG_DONTWAIT);
        if (bytesRead == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                _logger.logDebug("No more data to read (recv() returned EAGAIN or EWOULDBLOCK)");
                break;
            }
            else
            {
                _logger.logError("Failed to read from socket " + toString(_socket) + ": " + strerror(errno));
                break;
            }
        }
        else if (bytesRead == 0)
        {
            _logger.logInfo("Connection closed by peer on socket " + toString(_socket));
            break;
        }
        else
        {
            {
                std::string data(buffer.data(), bytesRead);
                _RequestParser->parse(data);
            }
            IResponse *response = 0;
            if (_RequestParser->isErroneous())
            {
                response = new ErrorResponse(_RequestParser->getErrorCode(), _RequestParser->getErrorMessage());
                _shouldClose = true;
            }
            else if (_RequestParser->isComplete())
            {
                IRequestContext ctx;
                response = _dispatcher.dispatch(_RequestParser, ctx);
            }
            if (response)
            {
                _responseQueue.push(response);
                modifyEpoll(_shouldClose ? EPOLLOUT | EPOLLET : EPOLLIN | EPOLLOUT | EPOLLET); // remove EPOLLIN on invalid request, keep EPOLLIN because of pipelining requests
                _logger.logDebug("Socket " + toString(_socket) + " set to writable");
            }
            else
            {
                _logger.logError("Failed to process request on socket " + toString(_socket));
                break;
            }
        }
    }
}

void Connection::onWritable()
{
    if (_responseQueue.empty())
    {
        modifyEpoll(EPOLLIN | EPOLLET); // remove EPOLLOUT
        return;
    }
    IResponse *response = _responseQueue.front();
    while (response->hasAvailableData())
    {
        std::string data = response->getNextData();
        ssize_t bytesSent = send(_socket, data.c_str(), data.size(), MSG_DONTWAIT);
        if (bytesSent == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                _logger.logDebug("No more data to send (send() returned EAGAIN or EWOULDBLOCK)");
                break;
            }
            else
            {
                _logger.logError("Failed to send data on socket " + toString(_socket) + ": " + strerror(errno));
                break;
            }
        }
    }
    if (response->isComplete())
    {
        _responseQueue.pop();
        delete response;
        if (_responseQueue.empty())
        {
            modifyEpoll(EPOLLIN | EPOLLET); // remove EPOLLOUT
            if (_shouldClose)
            {
                close(_socket);
                _socket = -1;
            }
        }
    }
    else
    {
        _logger.logDebug("Response not complete, waiting for more data");
    }
    
}