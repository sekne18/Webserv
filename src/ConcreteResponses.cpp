/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteResponses.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:33:42 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:49:21 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConcreteResponses.hpp"

ConcreteResponse::ConcreteResponse() : _code(200), _message("OK"), _body(), _transferEncoding(TE_NONE)
{}

ConcreteResponse::ConcreteResponse(size_t code, const std::string &message)
    : _code(code), _message(message), _body(), _transferEncoding(TE_NONE)
{
    if (code >= 400)
        _headers.push_back(std::make_pair("Connection", "close"));
}

ConcreteResponse::~ConcreteResponse() 
{}

size_t ConcreteResponse::getStatus() const 
{
    return _code;
}

const std::string &ConcreteResponse::getStatusMessage() const 
{
    return _message;
}

const std::vector<std::pair<std::string, std::string> > &ConcreteResponse::getHeaders() const 
{
    return _headers;
}

const std::string &ConcreteResponse::getBody() const 
{
    return _body;
}

void ConcreteResponse::setStatus(size_t status) 
{
    _code = status;
}

void ConcreteResponse::setStatusMessage(const std::string &statusMessage) 
{
    _message = statusMessage;
}

void ConcreteResponse::addHeader(const std::string &key, const std::string &value) 
{
    _headers.push_back(std::make_pair(key, value));
}

void ConcreteResponse::setBody(const std::string &body) 
{
    _body = body;
}

void ConcreteResponse::appendBody(const std::string &body) 
{
    _body += body;
}

std::string ConcreteResponse::getStatusLine() const
{
    return "HTTP/1.1 " + toString(_code) + " " + _message;
}

std::string ConcreteResponse::serializeHeaders() const
{
    std::string headers;
    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        headers += it->first + ": " + it->second + "\r\n";
    }
    return headers;
}

std::string ConcreteResponse::serialize() const
{
    std::string response = getStatusLine() + "\r\n";
    response += serializeHeaders();
    if (_transferEncoding == TE_NONE)
    {
        response += "Content-Length: " + toString(_body.size()) + "\r\n";
    }
    else
    {
        response += "Transfer-Encoding: chunked\r\n";
    }
    response += "\r\n";
    response += _body;
    return response;
}

