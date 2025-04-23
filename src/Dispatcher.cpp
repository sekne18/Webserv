/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:19:35 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 15:26:03 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dispatcher.hpp"

Dispatcher::Dispatcher(const ILogger &logger)
    : _logger(logger)
{
}

Dispatcher::~Dispatcher()
{
}

IResponse *Dispatcher::dispatch(const IRequestParser *request, IRequestContext &ctx)
{
    (void)request;
    (void)ctx;
    return new ErrorResponse(404, "Not Found");
}

void Dispatcher::loadFromConfig(const ServerConfig &config)
{
    // Load the config
    (void)config;
}