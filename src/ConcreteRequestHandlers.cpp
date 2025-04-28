/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteRequestHandlers.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:52:16 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 15:47:03 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConcreteRequestHandlers.hpp"

RedirectHandler::RedirectHandler(size_t code, const std::string &path)
	: _code(code), _path(path)
{
	
}

RedirectHandler::~RedirectHandler()
{
}

IResponse *RedirectHandler::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	(void)request;
	(void)ctx;
	if (_code >= 300 && _code < 400)
	{
		IResponse *response = new ConcreteResponse(_code, "redirect");
		response->addHeader("Location", _path);
		return response;
	}
	else
		return new ConcreteResponse(_code, getGenericStatusMessage(_code));
}

CgiHandler::CgiHandler(const std::string &inputPath, const std::string &outputPath)
	: _inputPath(inputPath), _outputPath(outputPath)
{
}

CgiHandler::~CgiHandler()
{
}

IResponse *CgiHandler::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	(void)request;
	(void)ctx;
	return new ConcreteResponse(200, "CGI Handler");
}

StaticFileHandler::StaticFileHandler(const std::string &inputPath, const std::string &outputPath)
	: _inputPath(inputPath), _outputPath(outputPath)
{
}

StaticFileHandler::~StaticFileHandler()
{
}

IResponse *StaticFileHandler::handle(const IRequestParser &request, IHandlerContext *ctx)
{
	(void)request;
	(void)ctx;
	IResponse *response = new ConcreteResponse(200, "OK");
	std::string path = _outputPath;
	if (path.empty())
		return new ConcreteResponse(500, "Internal Server Error");
	if (*(--path.end()) != '/')
		path += '/';
	path += std::string(request.getTarget()).substr(_inputPath.length());
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		int error = errno;
		if (file.bad())
		{
			return new ConcreteResponse(500, "Internal Server Error");
		}
		if (file.fail())
		{
			if (error == ENOENT)
			{
				return new ConcreteResponse(404, "Not Found");
			}
			else if (error == EACCES)
			{
				return new ConcreteResponse(403, "Forbidden");
			}
		}
		return new ConcreteResponse(400, "Bad request");
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	response->setBody(content);
	response->addHeader("Content-Type", "text/html");
	return response;
}