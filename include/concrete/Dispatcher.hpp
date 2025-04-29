/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:12:39 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 10:01:28 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <string>
#include <vector>
#include <map>

#include "ConcreteResponses.hpp"
#include "IDispatcher.hpp"
#include "ILogger.hpp"
#include "ServerConfig.hpp"
#include "ConcreteMiddleware.hpp"
#include "MiddlewareChainBuilder.hpp"
#include "ConcreteRequestHandlers.hpp"
#include "Utils.hpp"

struct virtualHost
{
  std::string ip;
  size_t port;
	std::vector<std::string> serverNames;
	std::map<std::string, IRequestHandler *> routes;
  std::map<size_t, std::string *> errorPages;
  size_t max_size;
};

/*
 * Dispatcher class:
 * Signleton class that handles the dispatching of requests to the appropriate handlers.
 */
class Dispatcher : public IDispatcher
{
  public:
    Dispatcher(const ILogger &logger);
    ~Dispatcher();

    IResponse *dispatch(IRequestParser *request, IRequestContext &ctx); // override
    /**
     * @brief Loads the configuration from the server config.
     * @param config The server configuration.
     * @details This method is used to load the configuration from the server config. It creates a new virtual host for each server block in the config.
     * it attaches the following middleware / handlers (in order) to the route:
     * - LoggingMiddleware
     * - SafeguardMiddleware
     * - DefaultErrorPageMiddleware
     * - LimitSizeMiddleware
     * - MethodFilterMiddleware
     * - RouteMiddleware
     * - DirectoryListingMiddleware
     * - CgiHandler (if the route is a CGI route)
     * - RedirectHandler (if the route is a redirect route)
     * - StaticFileHandler (if the route is a static file route)
     * @note The order of the middleware is important. The middleware is executed in the order it is added to the chain.
     */

    void loadFromConfig(const ServerConfig &config);
    /**
     * @brief Adds a route to the dispatcher.
     * @param ip The IP address of the virtual host.
     * @param port The port of the virtual host.
     * @param path The path of the route.
     * @param handler The handler for the route.
     * @details This method is used to add a route to the dispatcher. It creates a new route and adds it to the virtual host.
     * The virtual host must be created before adding a route (use loadFromConfig() to create the virtual host).
     * The handler is attached to obligated server middleware like logging, safeguard, default error page and limit size.
     * @note The order of the middleware is important. The middleware is executed in the order it is added to the chain.
     */
    void addRoute(const std::string &ip, size_t port, const std::string &path, IRequestHandler *handler);// override

  private:    
    const ILogger &_logger;
    std::vector<virtualHost> _virtualHosts;
};

#endif // DISPATCHER_HPP