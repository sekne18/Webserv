/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteResponses.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:22:47 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 16:27:52 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConcreteResponses.hpp"

ErrorResponse::ErrorResponse(size_t code, const std::string &message)
    : _code(code), _message(message)
{
}

ErrorResponse::~ErrorResponse()
{
}

std::string ErrorResponse::getStatus() const
{
    return "HTTP/1.1 " + toString(_code) + " " + _message + "\r\n";
}

std::string ErrorResponse::getHeaders() const
{
    return "Content-Type: text/html\r\nContent-Length: " \
    + toString(getNextData().size()) + "\r\n\r\n";
}

void ErrorResponse::fetchData()
{
    // No data to fetch
}

bool ErrorResponse::hasAvailableData() const
{
    return false;
}

std::string ErrorResponse::getNextData() const
{
    return getStatus();
}

void ErrorResponse::shiftData(size_t bytes)
{
    // No data to shift
}

bool ErrorResponse::isComplete() const
{
    return true;
}