/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:30:32 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:23:30 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(int epFd, t_socketInfo info, IDispatcher &dispatcher, const ILogger &logger, ISessionManager &sessionManager)
    : _epFd(epFd), _socket(info.fd), _ip(info.ip), _listenPort(info.listenPort), _port(info.port), _shouldClose(false),
      _RequestParser(new RequestParser(logger)), _responseWriter(new ResponseWriter()),
      _dispatcher(dispatcher), _logger(logger), _sessionManager(sessionManager)
{
}

void Connection::deleteResponseQueue()
{
    while (!_responseQueue.empty())
    {
        IResponse *response = _responseQueue.front();
        _responseQueue.pop();
        delete response;
    }
}

Connection::~Connection()
{
    if (_socket != -1) 
        close(_socket);
    delete _RequestParser;
    delete _responseWriter;
    if (_responseQueue.size() > 0)
        deleteResponseQueue();
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
            _shouldClose = true;
            modifyEpoll(EPOLLET);
            if (_responseQueue.size() > 0)
                deleteResponseQueue();
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
                response = new ConcreteResponse(_RequestParser->getErrorCode(), _RequestParser->getErrorMessage());
                _shouldClose = true;
            }
            if (_RequestParser->isComplete())
            {
                std::string _sessionId = _RequestParser->getSessionId();
                if (_sessionManager.isSessionExpired(_sessionId))
                    _sessionId = _sessionManager.createSession();
                RequestContext ctx(_listenPort, _ip, _sessionManager.getSession(_sessionId));
                if (!_RequestParser->isErroneous())
                {
                    response = _dispatcher.dispatch(_RequestParser, ctx);
                    _RequestParser->reset();
                    if (response->getStatus() >= 400)
                        _shouldClose = true;
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
}

void Connection::onWritable()
{
    if (_responseWriter->isComplete())
    {
        IResponse *response = _responseQueue.front();
        _responseWriter->start(*response);
    }
    while (!_responseWriter->isComplete())
    {
        std::string data = _responseWriter->getNextData(4096);
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
        else
        {
            _responseWriter->advanceData(bytesSent);
            _logger.logDebug("Sent " + toString(bytesSent) + " bytes on socket " + toString(_socket));
        }
    }
    if (_responseWriter->isComplete())
    {
        IResponse *response = _responseQueue.front();
        _logger.logDebug("Response sent: " + response->getStatusLine());
        _responseQueue.pop();
        delete response;
        if (_responseQueue.empty())
        {
            if (_shouldClose)
                modifyEpoll(EPOLLET);
            else
                modifyEpoll(EPOLLIN | EPOLLET); // remove EPOLLOUT
            return;
        }
    }
    else
    {
        _logger.logDebug("Response not complete, waiting for more data");
    }
    
}

bool Connection::shouldClose()
{
    return (_responseQueue.empty() && _shouldClose);
}