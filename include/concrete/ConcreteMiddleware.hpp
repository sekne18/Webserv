/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteMiddleware.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:45:06 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 15:09:11 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEMIDDLEWARE_HPP
#define CONCRETEMIDDLEWARE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "AMiddleware.hpp"
#include "ILogger.hpp"
#include "ConcreteResponses.hpp"
#include "RequestParser.hpp"
#include "HandlerContext.hpp"

class LimitSizeMiddleware : public AMiddleware
{
public:
	LimitSizeMiddleware(size_t maxSize);
	~LimitSizeMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	size_t _maxSize;
};

class SafeguardMiddleware : public AMiddleware
{
public:
	SafeguardMiddleware();
	~SafeguardMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
};

class DefaultErrorPageMiddleware : public AMiddleware
{
public:
	DefaultErrorPageMiddleware();
	~DefaultErrorPageMiddleware();

	DefaultErrorPageMiddleware &addDefaultErrorPage(size_t statusCode, const std::string *errorPage);
	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	std::map<size_t, const std::string *> _defaultErrorPages;
};

class MethodFilterMiddleware : public AMiddleware
{
public:
	MethodFilterMiddleware(const std::vector<std::string> &allowedMethods, size_t statusCode, const std::string &redirect = "");
	~MethodFilterMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	std::vector<std::string> _allowedMethods;
	size_t _statusCode;
	std::string _redirect;
};

class LoggingMiddleware : public AMiddleware
{
public:
	LoggingMiddleware(const ILogger &logger);
	~LoggingMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	const ILogger &_logger;
};

class RouteMiddleware : public AMiddleware
{
public:
	RouteMiddleware(const std::string &inPrefix, const std::string &outPrefix);
	~RouteMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	std::string _inPrefix;
	std::string _outPrefix;
};

class DirectoryListingMiddleware : public AMiddleware
{
public:
	DirectoryListingMiddleware(const std::string &option);
	~DirectoryListingMiddleware();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
private:
	enum Option
	{
		AUTO,
		OFF,
		ON
	};
	Option _option;
	std::string _path;
};

#endif // CONCRETEMIDDLEWARE_HPP