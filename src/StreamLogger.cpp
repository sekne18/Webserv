/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamLogger.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:38:47 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:30:48 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StreamLogger.hpp"

StreamLogger *StreamLogger::_instance = 0;

StreamLogger::StreamLogger()
    : _logStream(&std::cout), _logLevel(LOG_INFO)
{
}

StreamLogger *StreamLogger::getInstance()
{
    if (!_instance)
        _instance = new StreamLogger();
    return _instance;
}

void StreamLogger::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = 0;
    }
}

StreamLogger::~StreamLogger()
{
}

void StreamLogger::setLogLevel(LogLevel level)
{
    _logLevel = level;
}

void StreamLogger::setLogStream(std::ostream &stream)
{
    _logStream = &stream;
}

void StreamLogger::logError(const std::string &error) const
{
    if (!(_logLevel & LOG_ERROR))
        return;
    setConsoleColor("red");
    *_logStream << "Error: " << error << "\n";
    resetConsoleColor();
}

void StreamLogger::logInfo(const std::string &info) const
{
    if (!(_logLevel & LOG_INFO))
        return;
    *_logStream << "Info: " << info << "\n";
}

void StreamLogger::logDebug(const std::string &debug) const
{
    if (!(_logLevel & LOG_DEBUG))
        return;
    setConsoleColor("blue");
    *_logStream << "Debug: " << debug << "\n";
    resetConsoleColor();
}

void StreamLogger::logWarning(const std::string &warning) const
{
    if (!(_logLevel & LOG_WARNING))
        return;
    setConsoleColor("yellow");
    *_logStream << "Warning: " << warning << "\n";
    resetConsoleColor();
}

int StreamLogger::getColorCode(const std::string &color) const
{
    if (color == "red")
        return 31;
    else if (color == "green")
        return 32;
    else if (color == "yellow")
        return 33;
    else if (color == "blue")
        return 34;
    else if (color == "magenta")
        return 35;
    else if (color == "cyan")
        return 36;
    else if (color == "white")
        return 37;
    else
        return 0; // default color
}

void StreamLogger::setConsoleColor(const std::string &color) const
{
    //normally validate if _logStream is a console stream but funcion isatty is not allowed
    int code = getColorCode(color);
    if (code == 0)
        return;
    *_logStream << "\033[" << code << "m";
}

void StreamLogger::resetConsoleColor() const
{
    *_logStream << "\033[0m";
}
