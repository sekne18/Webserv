/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteMiddleware.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:45:06 by fmol              #+#    #+#             */
/*   Updated: 2025/04/16 15:34:17 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEMIDDLEWARE_HPP
#define CONCRETEMIDDLEWARE_HPP

#include <string>
#include <vector>

#include "IMiddleware.hpp"
#include "ILogger.hpp"

class MethodFilterMiddleware : public IMiddleware
{
public:
	MethodFilterMiddleware(const std::vector<std::string> &allowedMethods);
	~MethodFilterMiddleware();

	IResponse *handle(const IRequestParser &request, IHandlerContext &ctx, IRequestHandler *next); // override
};

class LoggingMiddleware : public IMiddleware
{
public:
	LoggingMiddleware(ILogger &logger);
	~LoggingMiddleware();

	IResponse *handle(const IRequestParser &request, IHandlerContext &ctx, IRequestHandler *next); // override
};

#endif // CONCRETEMIDDLEWARE_HPP