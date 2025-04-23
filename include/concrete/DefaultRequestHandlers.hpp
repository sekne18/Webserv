/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultRequestHandlers.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:42:27 by fmol              #+#    #+#             */
/*   Updated: 2025/04/16 14:47:21 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTREQUESTHANDLERS_HPP
#define DEFAULTREQUESTHANDLERS_HPP

#include "IRequestHandler.hpp"

class RedirectHandler : public IRequestHandler
{
public:
	RedirectHandler(size_t code, const std::string &path);
	~RedirectHandler();

	IResponse *handleRequest(const IRequestParser *request, IHandlerContext &ctx); // override
};

class CgiHandler : public IRequestHandler
{
public:
	CgiHandler(const std::string &path);
	~CgiHandler();

	IResponse *handleRequest(const IRequestParser *request, IHandlerContext &ctx); // override
};

class StaticFileHandler : public IRequestHandler
{
public:
	StaticFileHandler(const std::string &path);
	~StaticFileHandler();

	IResponse *handleRequest(const IRequestParser *request, IHandlerContext &ctx); // override
};

#endif // DEFAULTREQUESTHANDLERS_HPP