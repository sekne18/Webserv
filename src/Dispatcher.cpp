/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:19:35 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 14:28:09 by fmol             ###   ########.fr       */
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

IResponse *Dispatcher::dispatch(const IRequestParser *request, IRequestContext &ctx)
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
        return match->routes.begin()->second->handle(*request, 0);
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
        vh.serverNames = it->serverNames; // -> make a structure with server names within the virtual host
        for (std::map<size_t, std::string *>::const_iterator it2 = it->errorPages.begin(); it2 != it->errorPages.end(); ++it2)
        {
            vh.errorPages[it2->first] = it2->second;
        }
        // -> make a structure with the routes within the server name
        MiddlewareChainBuilder builder;
        for (std::vector<Route>::const_iterator it2 = it->routes.begin(); it2 != it->routes.end(); ++it2)
        {
            //TODO: do not do this for empty routes so devs can add their own routes programatically
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
                .use(new MethodFilterMiddleware(it2->allowedMethods, it2->disallowedReturnCode, it2->disallowedPath))
                .use(new DirectoryListingMiddleware(it2->index));
            if (it2->isCgi)
                builder.handle(new CgiHandler(it2->locationPath, it2->root));
            else if (it2->isReturn)
                builder.handle(new RedirectHandler(it2->returnCode, it2->returnPath));
            else
                builder.handle(new StaticFileHandler(it2->locationPath, it2->root));
            vh.routes[it2->locationPath] = builder.build();
            builder.clear();
        }
        _virtualHosts.push_back(vh);
    }
    // -> construct the middleware chain for each route
    //   -> loggingMiddleware: for logging the request and response (calls next handler first and logs the request and response)
    //   -> safeguardMiddleware: for handling the errors (calls next handler first and sets the response to 500 if there is no response)
    //   -> defaultErrorPageMiddleware: for setting the body on errorResponses (calls next handler first and sets the body on response)
    //   -> limitSizeMiddleware: for limiting the size of the request (only calls next handler if the size is ok)
    //   -> maxSizeMiddleware: for limiting the size of the request (only calls next handler if the size is ok)
    //   -> methodFilterMiddleware: for filtering the methods (only calls next handler if the method is ok)
    //   -> directoryListingMiddleware: for handling the directory listing (only calls next handler if the request is ok)
    //      -> cgiHandler: for handling the cgi
    //      -> staticFileHandler: for handling the static files
    //      -> redirectHandler: for handling the redirect

}