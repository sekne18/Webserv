/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:01:21 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:55:51 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkManager.hpp"

NetworkManager::NetworkManager(const ILogger &logger, ISessionManager &sessionManager, size_t maxEvents)
    : _shouldStop(false), _maxEvents(maxEvents), _epFd(-1), _dispatcher(0), _logger(logger), _sessionManager(sessionManager)
{
    _epFd = epoll_create1(0);
    if (_epFd == -1)
        throw std::runtime_error("Failed to create epoll file descriptor");
}

NetworkManager::~NetworkManager()
{
    while (!_connections.empty())
    {
        unregisterSocket(_connections.begin()->first);
    }
    for (std::vector<t_socketInfo>::iterator it = _listeners.begin(); it != _listeners.end(); ++it)
    {
        unregisterSocket(it->fd);
        close(it->fd);
    }
    _listeners.clear();
    if (_epFd != -1)
    {
        close(_epFd);
        _epFd = -1;
    }
    _logger.logInfo("NetworkManager stopped");
}

void NetworkManager::setDispatcher(IDispatcher *dispatcher)
{
    _dispatcher = dispatcher;
}

int NetworkManager::createSocket(const std::string &ip, size_t port)
{
    for (std::vector<t_socketInfo>::const_iterator it = _listeners.begin(); it != _listeners.end(); ++it)
    {
        if (isMatchInterface(it->ip, it->port, ip, port))
            throw SocketAlreadyInUseException("Socket already in use: " + ip + ":" + toString(port));
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ((ip == "any") ? INADDR_ANY : inet_addr(ip.c_str()));
    if (addr.sin_addr.s_addr == INADDR_NONE)
        throw std::runtime_error("Invalid IP address: " + ip);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    // makes sure the socket is opened even when it is in TIME_WAIT state
    int optval = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        close(fd);
        throw std::runtime_error("Failed to set socket options: " + std::string(strerror(errno)));
    }
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(fd);
        throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
    }
    return (fd);
}

void NetworkManager::listenOn(const std::string &ip, size_t port)
{

    t_socketInfo l;
    l.ip = ip;
    l.port = port;
    l.listenPort = port;
    try
    {
        l.fd = createSocket(ip, port);
    }
    catch (const SocketAlreadyInUseException &e)
    {
        _logger.logWarning(e.what());
        return;
    }
    if (listen(l.fd, SOMAXCONN) == -1)
    {
        close(l.fd);
        throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
    }
    registerSocket(l.fd);
    _listeners.push_back(l);
    _logger.logInfo("Listening on " + ip + ":" + toString(port));
}

void NetworkManager::registerSocket(int fd)
{
    assert(_epFd != -1, "Epoll file descriptor is not valid");
    assert(fd >= 0, "Invalid file descriptor");
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(fd);
        throw std::runtime_error("Failed to set socket to non-blocking: " + std::string(strerror(errno)));
    }
    // Set the socket to close on exec -- is this useful??
    if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
    {
        close(fd);
        throw std::runtime_error("Failed to set socket to close on exec: " + std::string(strerror(errno)));
    }
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        close(fd);
        throw std::runtime_error("Failed to register socket: " + std::string(strerror(errno)));
    }
    _logger.logInfo("Socket " + toString(fd) + " registered");
}

void NetworkManager::unregisterSocket(int fd)
{
    assert(_epFd != -1, "Epoll file descriptor is not valid");
    assert(fd >= 0, "Invalid file descriptor");
    if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, 0) == -1)
    {
        switch (errno)
        {
        case ENOENT:
            _logger.logError("Socket " + toString(fd) + " not found in epoll");
            break;
        default:
            _logger.logError("Failed to unregister socket: " + std::string(strerror(errno)));
            break;
        }
    }
    std::map<int, IConnection *>::iterator it = _connections.find(fd);
    if (it != _connections.end())
    {
        delete it->second;
        _connections.erase(fd);
        _logger.logInfo("Socket " + toString(fd) + " unregistered");
    }
    else
        _logger.logDebug("Socket " + toString(fd) + " not found in connections");
}

void NetworkManager::stop()
{
    _shouldStop = true;
}

void NetworkManager::run()
{
    std::vector<struct epoll_event> events(_maxEvents);
    while (!_shouldStop)
    {
        _logger.logDebug("Waiting for events...");
        int nrEvents = epoll_wait(_epFd, events.data(), _maxEvents, -1);
        if (nrEvents == -1)
        {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("epoll_wait failed" + std::string(strerror(errno)));
        }
        for (int i = 0; i < nrEvents; ++i)
        {
            if (events[i].events & EPOLLERR)
            {
                _logger.logError("EPOLLERR on socket " + toString(events[i].data.fd));
                unregisterSocket(events[i].data.fd);
            }
            else if (events[i].events & EPOLLHUP)
            {
                _logger.logInfo("EPOLLHUP on socket " + toString(events[i].data.fd));
                unregisterSocket(events[i].data.fd);
            }
            else if (events[i].events & EPOLLIN)
            {
                if (_connections.find(events[i].data.fd) != _connections.end())
                {
                    _connections[events[i].data.fd]->onReadable();
                    if (_connections[events[i].data.fd]->shouldClose())
                    {
                        unregisterSocket(events[i].data.fd);
                        _logger.logInfo("Connection closed on socket " + toString(events[i].data.fd));
                    }
                }
                else if (std::find(_listeners.begin(), _listeners.end(), events[i].data.fd) != _listeners.end())
                {
                    struct sockaddr_in addr;
                    socklen_t addrLen = sizeof(addr);
                    int newFd = accept(events[i].data.fd, (struct sockaddr *)&addr, &addrLen);
                    if (newFd == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            _logger.logDebug("No more connections to accept (accept() returned EAGAIN or EWOULDBLOCK)");
                            continue;
                        }
                        _logger.logError("Failed to accept connection" + std::string(strerror(errno)));
                    }
                    else
                    {
                        registerSocket(newFd);
                        try
                        {
                            t_socketInfo info;
                            info.fd = newFd;
                            info.ip = inet_ntoa(addr.sin_addr);
                            info.port = ntohs(addr.sin_port);
                            info.listenPort = std::find(_listeners.begin(), _listeners.end(), events[i].data.fd)->listenPort;
                            _connections[newFd] = new Connection(_epFd, info, *_dispatcher, _logger, _sessionManager);
                        }
                        catch (std::bad_alloc &e)
                        {
                            _logger.logError("Failed to allocate memory for connection" + std::string(strerror(errno)));
                            close(newFd);
                            throw std::runtime_error("Failed to allocate memory for connection" + std::string(strerror(errno)));
                        }

                        _logger.logInfo("Accepted connection on socket " + toString(newFd));
                    }
                }
                else
                    _logger.logError("EPOLLIN on unknown socket " + toString(events[i].data.fd));
            }
            else if (events[i].events & EPOLLOUT)
            {
                if (_connections.find(events[i].data.fd) != _connections.end())
                {
                    _connections[events[i].data.fd]->onWritable();
                    if (_connections[events[i].data.fd]->shouldClose())
                    {
                        unregisterSocket(events[i].data.fd);
                        _logger.logInfo("Connection closed on socket " + toString(events[i].data.fd));
                    }
                }
                else
                    _logger.logError("EPOLLOUT on unknown socket " + toString(events[i].data.fd));
            }
        }
        // TODO: Handle other external events (timed events, etc.)
    }
}

NetworkManager::SocketAlreadyInUseException::SocketAlreadyInUseException(const std::string &msg)
    : std::exception(), _msg(msg)
{
}

const char *NetworkManager::SocketAlreadyInUseException::what() const throw()
{
    return _msg.c_str();
}