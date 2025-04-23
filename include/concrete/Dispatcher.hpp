/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:12:39 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 15:24:44 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "ConcreteResponses.hpp"
#include "IDispatcher.hpp"
#include "ILogger.hpp"
#include "ServerConfig.hpp"

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
};

#endif // DISPATCHER_HPP