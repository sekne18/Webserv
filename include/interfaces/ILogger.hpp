/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ILogger.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:29:05 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:32:39 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#include "IRequestParser.hpp"
#include "IResponse.hpp"

class ILogger
{
  public:
    virtual ~ILogger() {};

    enum LogLevel
    {
        LOG_INFO = 1,
        LOG_DEBUG = 2,
        LOG_WARNING = 4,
        LOG_ERROR = 8,
    };
    virtual void setLogLevel(LogLevel level) = 0;

    virtual void logError(const std::string &error) const = 0;
    virtual void logInfo(const std::string &info) const = 0;
    virtual void logDebug(const std::string &debug) const = 0;
    virtual void logWarning(const std::string &warning) const = 0;
};

ILogger::LogLevel operator|(ILogger::LogLevel lhs, ILogger::LogLevel rhs);
ILogger::LogLevel operator|=(ILogger::LogLevel &lhs, ILogger::LogLevel rhs);

#endif // LOGGER_HPP