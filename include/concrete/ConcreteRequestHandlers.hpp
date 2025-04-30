/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteRequestHandlers.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:42:27 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:37:26 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEREQUESTHANDLERS_HPP
#define CONCRETEREQUESTHANDLERS_HPP

#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "ConcreteResponses.hpp"
#include "IRequestHandler.hpp"
#include "IRequestParser.hpp"

class RedirectHandler : public IRequestHandler
{
  public:
    RedirectHandler(size_t code, const std::string &path);
    ~RedirectHandler();

    IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
  private:
    size_t _code;
    std::string _path;
};

class CgiHandler : public IRequestHandler
{
  public:
    CgiHandler(const std::string &extension);
    ~CgiHandler();

    IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
  private:
    IResponse *execute(IRequestParser &request, IHandlerContext *ctx);
    std::string _extension;
};

class StaticFileHandler : public IRequestHandler
{
  public:
    StaticFileHandler();
    ~StaticFileHandler();

    IResponse *handle(IRequestParser &request, IHandlerContext *ctx); // override
  private:
    IResponse *handleGET(IRequestParser &request, IHandlerContext *ctx);
    IResponse *handlePOST(IRequestParser &request, IHandlerContext *ctx);
    IResponse *handleDELETE(IRequestParser &request, IHandlerContext *ctx);
};

#endif // CONCRETEREQUESTHANDLERS_HPP