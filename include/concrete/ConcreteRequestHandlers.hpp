/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteRequestHandlers.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:42:27 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 13:30:50 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEREQUESTHANDLERS_HPP
#define CONCRETEREQUESTHANDLERS_HPP

#include <string>
#include <fstream>

#include "IRequestHandler.hpp"
#include "IRequestParser.hpp"
#include "ConcreteResponses.hpp"

class RedirectHandler : public IRequestHandler
{
public:
	RedirectHandler(size_t code, const std::string &path);
	~RedirectHandler();

	IResponse *handle(const IRequestParser &request, IHandlerContext *ctx); // override
private:
	size_t _code;
	std::string _path;
};

class CgiHandler : public IRequestHandler
{
public:
	CgiHandler(const std::string &inputPath, const std::string &outputPath);
	~CgiHandler();

	IResponse *handle(const IRequestParser &request, IHandlerContext *ctx); // override
private:
	std::string _inputPath;
	std::string _outputPath;
};

class StaticFileHandler : public IRequestHandler
{
public:
	StaticFileHandler(const std::string &inputPath, const std::string &outputPath);
	~StaticFileHandler();

	IResponse *handle(const IRequestParser &request, IHandlerContext *ctx); // override
private:
	std::string _inputPath;
	std::string _outputPath;
};

#endif // CONCRETEREQUESTHANDLERS_HPP