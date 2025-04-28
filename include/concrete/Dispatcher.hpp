/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:12:39 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 10:25:21 by fmol             ###   ########.fr       */
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

    IResponse *dispatch(const IRequestParser *request, IRequestContext &ctx); // override
    void loadFromConfig(const ServerConfig &config);

  private:
    const ILogger &_logger;
    std::vector<virtualHost> _virtualHosts;
};

#endif // DISPATCHER_HPP