/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamLogger.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:31:00 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:30:59 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STREAMLOGGER_HPP
#define STREAMLOGGER_HPP

#include <iostream>

#include "ILogger.hpp"

class StreamLogger : public ILogger
{
public:
	static StreamLogger *getInstance();
	static void destroyInstance();

	void setLogLevel(LogLevel level);
	void setLogStream(std::ostream &stream);

	void logError(const std::string &error) const;
	void logInfo(const std::string &info) const;
	void logDebug(const std::string &debug) const;
	void logWarning(const std::string &warning) const;

private:
	int getColorCode(const std::string &color) const;
	void setConsoleColor(const std::string &color) const;
	void resetConsoleColor() const;
	StreamLogger();
	~StreamLogger();

	static StreamLogger *_instance;
	std::ostream *_logStream;
	LogLevel _logLevel;
};

#endif // STREAMLOGGER_HPP