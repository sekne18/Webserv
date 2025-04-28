/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteMiddleware.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:59:25 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 16:07:58 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConcreteMiddleware.hpp"

LimitSizeMiddleware::LimitSizeMiddleware(size_t maxSize)
	: AMiddleware(), _maxSize(maxSize)
{
}

LimitSizeMiddleware::~LimitSizeMiddleware()
{
}

IResponse *LimitSizeMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	if (request.getBody().size() > _maxSize)
	{
		return new ConcreteResponse(413, "Payload Too Large");
	}
	IResponse *response = 0;
	if (_next)
	{
		response = _next->handle(request, ctx);
	}
	return response;
}

SafeguardMiddleware::SafeguardMiddleware()
	: AMiddleware()
{
}

SafeguardMiddleware::~SafeguardMiddleware()
{
}

IResponse *SafeguardMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	IResponse *response = 0;
	if (_next)
	{
		response = _next->handle(request, ctx);
	}
	if (!response)
	{
		return new ConcreteResponse(500, "Internal Server Error");
	}
	return response;
}

DefaultErrorPageMiddleware::DefaultErrorPageMiddleware()
	: AMiddleware()
{
}

DefaultErrorPageMiddleware::~DefaultErrorPageMiddleware()
{
}

DefaultErrorPageMiddleware &DefaultErrorPageMiddleware::addDefaultErrorPage(size_t statusCode, const std::string *errorPage)
{
	_defaultErrorPages[statusCode] = errorPage;
	return *this;
}

IResponse *DefaultErrorPageMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	IResponse *response = 0;
	if (_next)
	{
		response = _next->handle(request, ctx);
		if (response && _defaultErrorPages.find(response->getStatus()) != _defaultErrorPages.end())
		{
			response->setBody(*(_defaultErrorPages[response->getStatus()]));
		}
	}
	return response;
}

MethodFilterMiddleware::MethodFilterMiddleware(const std::vector<std::string> &allowedMethods, size_t statusCode, const std::string &redirect)
	: AMiddleware(), _allowedMethods(allowedMethods), _statusCode(statusCode), _redirect(redirect)
{
}

MethodFilterMiddleware::~MethodFilterMiddleware()
{
}

IResponse *MethodFilterMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	if (std::find(_allowedMethods.begin(), _allowedMethods.end(), request.getMethod()) == _allowedMethods.end())
	{
		if (_statusCode >= 300 && _statusCode < 400)
		{
			IResponse *response = new ConcreteResponse(_statusCode, "redirect");
			response->addHeader("Location", _redirect);
			return response;
		}
		else
		{
			return new ConcreteResponse(_statusCode, "Method Not Allowed");
		}
	}
	if (_next)
	{
		return _next->handle(request, ctx);
	}
	return 0;
}

LoggingMiddleware::LoggingMiddleware(const ILogger &logger)
	: AMiddleware(), _logger(logger)
{
}

LoggingMiddleware::~LoggingMiddleware()
{
}

IResponse *LoggingMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	_logger.logDebug("Request: " + request.getMethod() + " " + request.getTarget());
	IResponse *response = 0;
	if (_next)
	{
		response = _next->handle(request, ctx);
		if (response)
			_logger.logDebug("Response: " + toString(response->getStatus()));
		else
			_logger.logDebug("Response: No response");
	}
	return response;
}

RouteMiddleware::RouteMiddleware(const std::string &inPrefix, const std::string &outPrefix)
	: AMiddleware(), _inPrefix(inPrefix), _outPrefix(outPrefix)
{
}

RouteMiddleware::~RouteMiddleware()
{
}

DirectoryListingMiddleware::DirectoryListingMiddleware(const std::string &option)
	: AMiddleware()
{
	if (option == "auto")
		_option = AUTO;
	else if (option == "off")
		_option = OFF;
	else
	{
		_option = ON;
		_path = option;
	}
}

DirectoryListingMiddleware::~DirectoryListingMiddleware()
{
}

IResponse *DirectoryListingMiddleware::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	IResponse *response = 0;
	if (request.getTarget().find("..") != std::string::npos)
	{
		return new ConcreteResponse(403, "Forbidden: possible path traversal");
	}
	if (*(request.getTarget().rbegin()) != '/')
	{
		try
		{
			if (!isDirectory(request.getTarget()))
			{
				if (_next)
					return _next->handle(request, ctx);
				else
					return new ConcreteResponse(500, "Internal Server Error");
			}
		}
		catch(const std::exception& e)
		{
			return new ConcreteResponse(500, "Internal Server Error");
		}

	}
	if (_option == AUTO)
	{
		response = new ConcreteResponse(200, "Directory Listing: auto-generated");
		response->setBody(generateDirectoryListing(request.getTarget()));
	}
	else if (_option == ON)
	{
		response = new ConcreteResponse(200, "Directory Listing: " + _path);
		std::string *content = loadFile(_path);
		response->setBody(*content);
		delete content;
	}
	else
	{
		response = new ConcreteResponse(403, "Forbidden");
	}
	return response;
}