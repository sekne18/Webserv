/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:19:35 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 15:14:58 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dispatcher.hpp"

Dispatcher::Dispatcher(const ILogger &logger)
    : _logger(logger)
{
}

Dispatcher::~Dispatcher()
{
    for (std::vector<virtualHost>::iterator it = _virtualHosts.begin(); it != _virtualHosts.end(); ++it)
    {
        for (std::map<std::string, IRequestHandler *>::iterator it2 = it->routes.begin(); it2 != it->routes.end(); ++it2)
        {
            delete it2->second;
        }
        it->routes.clear();
        for (std::map<size_t, std::string *>::iterator it2 = it->errorPages.begin(); it2 != it->errorPages.end(); ++it2)
        {
            delete it2->second;
        }
        it->errorPages.clear();
    }
    _virtualHosts.clear();
}

IResponse *Dispatcher::dispatch(IRequestParser *request, IRequestContext &ctx)
{
    std::vector<std::vector<virtualHost>::reverse_iterator> matchingHosts;
    for (std::vector<virtualHost>::reverse_iterator it = _virtualHosts.rbegin(); it != _virtualHosts.rend(); ++it)
    {
        if (it->ip == ctx.getIp() && it->port == ctx.getPort())
        {
            matchingHosts.push_back(it);
        }
        else if (it->ip == "any" && it->port == ctx.getPort())
        {
            matchingHosts.push_back(it);
        }
    }
    if (matchingHosts.empty()) //should not happen
        return new ConcreteResponse(500, "Internal Server Error");
    virtualHost *match = &*(matchingHosts.back());
    for (std::vector<std::vector<virtualHost>::reverse_iterator>::iterator it = matchingHosts.begin(); it != matchingHosts.end(); ++it)
    {
        for (std::vector<std::string>::const_iterator it2 = (*it)->serverNames.begin(); it2 != (*it)->serverNames.end(); ++it2)
        {
            if (*it2 == request->getHost())
            {
                match = &**it;
                break;
            }
        }
    }
    if (match)
    {
        size_t longestMatch = 0;
        IRequestHandler *handler = 0;
        for (std::map<std::string, IRequestHandler *>::iterator it = match->routes.begin(); it != match->routes.end(); ++it)
        {
            if (isCGIMatch(it->first, request->getTarget()))
            {
                handler = it->second;
                break;
            }
            if (request->getTarget().find(it->first) == 0) //extension check for cgi
            {
                if (it->first.size() > longestMatch)
                {
                    longestMatch = it->first.size();
                    handler = it->second;
                }
            }
        }
        HandlerContext hCtx(ctx.getSession());
        if (!handler)
            return match->routes.begin()->second->handle(*request, &hCtx);
        else
            return handler->handle(*request, &hCtx);
    }
    return new ConcreteResponse(404, "Not Found");
}


void Dispatcher::loadFromConfig(const ServerConfig &config)
{
    // Load the config
    (void)config;
    //make a structure you can iterate over for the virtual hosts
    const std::vector<ServerData> &servers = config.getServerData();
    for (std::vector<ServerData>::const_iterator it = servers.begin(); it != servers.end(); ++it)
    {
        virtualHost vh;
        vh.ip = it->ip;
        vh.port = it->port;
        vh.max_size = it->max_size;
        vh.serverNames = it->serverNames; // -> make a structure with server names within the virtual host
        for (std::map<size_t, std::string *>::const_iterator it2 = it->errorPages.begin(); it2 != it->errorPages.end(); ++it2)
        {
            vh.errorPages[it2->first] = it2->second;
        }
        // -> make a structure with the routes within the server name
        MiddlewareChainBuilder builder;
        for (std::vector<Route>::const_iterator it2 = it->routes.begin(); it2 != it->routes.end(); ++it2)
        {
            DefaultErrorPageMiddleware *errorMid = new DefaultErrorPageMiddleware();
            for (std::map<size_t, std::string *>::const_iterator it3 = vh.errorPages.begin(); it3 != vh.errorPages.end(); ++it3)
            {
                errorMid->addDefaultErrorPage(it3->first, it3->second);
            }
            builder
                .use(new LoggingMiddleware(_logger))
                .use(new SafeguardMiddleware())
                .use(errorMid)
                .use(new LimitSizeMiddleware(it->max_size))
                .use(new MethodFilterMiddleware(it2->allowedMethods, it2->disallowedReturnCode, it2->disallowedPath));
            if (it2->isCgi)
            {
                std::string prefix = it2->locationPath;
                std::string extension = prefix.substr(prefix.find_last_of('.'));
                prefix = prefix.substr(0, prefix.find_last_of('.'));
                if (prefix.find('*') != std::string::npos)
                {
                    prefix = prefix.substr(0, prefix.find('*'));
                }
                builder.use(new RouteMiddleware(prefix, it2->root))
                .use(new DirectoryListingMiddleware(it2->index));
                builder.handle(new CgiHandler(extension));
            }
            else
            {
                builder.use(new RouteMiddleware(it2->locationPath, it2->root))
                .use(new DirectoryListingMiddleware(it2->index));
            }

            if (it2->isReturn)
                builder.handle(new RedirectHandler(it2->returnCode, it2->returnPath));
            else if (!it2->isCgi)
                builder.handle(new StaticFileHandler());
            vh.routes[it2->locationPath] = builder.build();
            builder.clear();
        }
        _virtualHosts.push_back(vh);
    }
}

void Dispatcher::addRoute(const std::string &ip, size_t port, const std::string &path, IRequestHandler *handler)
{
    MiddlewareChainBuilder builder;
    DefaultErrorPageMiddleware *errorMid = new DefaultErrorPageMiddleware();
    virtualHost *vh;
    for (std::vector<virtualHost>::iterator it = _virtualHosts.begin(); it != _virtualHosts.end(); ++it)
    {
        if (it->ip == ip && it->port == port)
        {
            vh = &*it;
            break;
        }
    }
    if (vh->ip.empty())
        throw std::runtime_error("No matching virtual host found for: " + ip + ":" + toString(port));
    if (vh->routes.find(path) != vh->routes.end())
        throw std::runtime_error("Route already exists: " + path);
    for (std::map<size_t, std::string *>::const_iterator it3 = vh->errorPages.begin(); it3 != vh->errorPages.end(); ++it3)
    {
        errorMid->addDefaultErrorPage(it3->first, it3->second);
    }
    builder
        .use(new LoggingMiddleware(_logger))
        .use(new SafeguardMiddleware())
        .use(errorMid)
        .use(new LimitSizeMiddleware(vh->max_size))
        .handle(handler);
    vh->routes[path] = builder.build();
}
    