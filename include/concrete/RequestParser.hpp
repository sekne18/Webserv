/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:34:29 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 15:05:16 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <map>
#include <stdexcept>

#include "MessageException.hpp"
#include "IRequestParser.hpp"
#include "ILogger.hpp"
#include "Validators.hpp"


class RequestParser : public IRequestParser
{
  public:
    RequestParser(const ILogger &logger);
    ~RequestParser();

    void parse(const std::string &data);                          // override;
    bool isComplete() const;                                      // override;
	bool isErroneous() const;									// override;
	const std::string &getErrorMessage() const;					// override;
	size_t getErrorCode() const;									// override;
    const std::string &getMethod() const;                         // override;
    const std::string &getTarget() const;                           // override;
    const std::string &getVersion() const;                        // override;
    const std::multimap<std::string, std::string> &getHeaders() const; // override;
    const std::string &getBody() const;                           // override;
    void reset();                                                 // override;
    void flushBuffer();                                           // override;
  private:
    void parseLine(const std::string &line);
    void parseStartLine(const std::string &line);
    void parseHeaders(const std::string &line);
    void parseBody(const std::string &line);
    void parseChunkedBody(const std::string &line);
	bool isBodyExpected() const;
	void setErrorState(const std::string &errMsg, size_t errCode);

    const ILogger &_logger;
    enum State
    {
        START_1,
        START_2,
        HEADERS,
        BODY,
        CHUNKED,
        COMPLETE,
        ERROR
    };
    State _state;
	std::string _errMsg;
	size_t _errCode;
    std::string _buffer;
    std::string _method;
	std::string _host;
    std::string _target;
    std::string _version;
    std::multimap<std::string, std::string> _headers;
    std::string _body;
    bool _isComplete;
    bool _isChunked;
    size_t _contentLength;
    size_t _contentLengthRead;
};

#endif // REQUESTPARSER_HPP