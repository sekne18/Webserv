/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:29 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 09:24:51 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser(const ILogger &logger)
    : _logger(logger),
      _state(START_1),
      _errCode(0),
      _isComplete(false),
      _isChunked(false),
      _noBody(true),
      _contentLength(0)
{
}

RequestParser::~RequestParser()
{
}

RequestParser::RequestParser(const RequestParser &other)
    : _logger(other._logger),
      _state(other._state),
      _errMsg(other._errMsg),
      _errCode(other._errCode),
      _buffer(other._buffer),
      _method(other._method),
      _host(other._host),
      _target(other._target),
      _version(other._version),
      _headers(other._headers),
      _body(other._body),
      _isComplete(other._isComplete),
      _isChunked(other._isChunked),
      _noBody(other._noBody),
      _contentLength(other._contentLength)
{
}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        _state = other._state;
        _errMsg = other._errMsg;
        _errCode = other._errCode;
        _buffer = other._buffer;
        _method = other._method;
        _host = other._host;
        _target = other._target;
        _version = other._version;
        _headers = other._headers;
        _body = other._body;
        _isComplete = other._isComplete;
        _isChunked = other._isChunked;
        _noBody = other._noBody;
        _contentLength = other._contentLength;
    }
    return *this;
}

void RequestParser::setErrorState(const std::string &errMsg, size_t errCode)
{
    _errMsg = errMsg;
    _errCode = errCode;
    _state = ERROR;
    _isComplete = true;
    _logger.logError(errMsg + " (code: " + toString(errCode) + ")");
}

const std::string &RequestParser::getHost() const
{
    return _host;
}

void RequestParser::parse(const std::string &data)
{
    _logger.logDebug("Parsing data: " + data);
    _buffer += data;
    while (true)
    {
        if (_state == COMPLETE || _state == ERROR)
            return;
        size_t pos = _buffer.find("\r\n");
        if (pos == std::string::npos)
            return;
        else if (pos == 0 && _state == START_1) //ignore at least 1 empty line (just 1 CRLF) prior to the request-line (for robustness)
        {
            _buffer.erase(0, 2);
            _state = START_2;
            _logger.logDebug("Ignoring empty line (CRLF) at start of request. (state => START_2)");
            continue;
        }
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 2);
        //bare CR not in content => consider this request as invalid
        pos = line.find("\r");
        if (pos != std::string::npos && _state != BODY)
        {
            setErrorState("Bare CR not contained within content", 400);
            return;
        }
        parseLine(line);
    }
    

    //if whitespace between start-line and first header field => consider this request as invalid
    // -> check in parseStartLine at end
    // -> check in parseHeaders at start

    //if other octets that are non-conforming to http grammar => send 400 Bad Request and close the connection

    // recommended: be able to process request-line of min 8000 octets
    //state machine:
    //START: ignore first CRLF -- done
    //START -> METHOD: read method (GET, POST, DELETE) (case sensitive) -- done
    // if method not implemented / longer than we want to handle => send 501 Not Implemented and close the connection -- done
    //METHOD -> TARGET: read target (path) -- done
    // if target longer than we want to handle => send 414 Request-URI Too Long and close the connection -- done
    // if target contains invalid characters (like space) => send 400 Bad Request and close the connection -- done
    //TARGET -> VERSION: read version (HTTP/1.1)
    //VERSION -> HEADERS: read headers (key: value)
    // if multiple HOST headers => send 400 Bad Request and close the connection
    // if HOST header is missing => send 400 Bad Request and close the connection
    // if HOST header field-value is invalid => send 400 Bad Request and close the connection
    // HEADER definition: 'key' ':' 'OWS' 'value' 'OWS' (key = case insensitive, OWS = optional whitespace)
    // -> if whitespace between key and ':' => send 400 Bad Request and close the connection
    // if obsolete line folding (multiple lines for one header) and media type is not message/http => send 400 Bad Request (with explanation) and close the connection
    // if Transfer-Encoding header is present and value is not 'chunked' or 'identity' => send 501 Not Implemented and close the connection
    // if Content-Length and Transfer-Encoding header are both present => send 400 Bad Request and close the connection
    //HEADERS -> BODY: read body (if Content-Length / Transfer-Encoding header is present)
    // if no Content-Length header and no Transfer-Encoding header => send  411 Length Required and close the connection
    //HEADERS -> COMPLETE: end of headers (no message body)
    //HEADERS -> CHUNKED: read chunked body (if Transfer-Encoding header is present)
    //BODY -> COMPLETE: end of request
    //COMPLETE -> START: reset parser
}

void RequestParser::parseLine(const std::string &line)
{
    switch (_state)
    {
        case START_1:
            parseStartLine(line);
            break;
        case START_2:
            parseStartLine(line);
            break;
        case HEADERS:
            parseHeaders(line);
            break;
        case BODY:
            if (_isChunked)
                parseChunkedBody(line);
            else
                parseBody(line);
            break;
        case CHUNKED:
            parseChunkedBody(line);
            break;
        case ERROR:
            // already in error state, ignore the rest
            break;
        case COMPLETE:
            // already complete, ignore the rest
            break;
        default:
            // should never happen
            _logger.logError("Unknown state in RequestParser: " + toString(_state));
            break;
    }
}

bool RequestParser::isErroneous() const
{
    return _state == ERROR;
}

const std::string &RequestParser::getErrorMessage() const
{
    return _errMsg;
}

size_t RequestParser::getErrorCode() const
{
    return _errCode;
}

bool RequestParser::isBodyExpected() const
{
    return ((_isChunked || _contentLength > 0) && !_noBody);
}


bool RequestParser::isComplete() const
{
    return _isComplete;
}

const std::string &RequestParser::getMethod() const
{
    return _method;
}

const std::string &RequestParser::getTarget() const
{
    return _target;
}

void RequestParser::setTarget(const std::string &target)
{
    _target = target;
}

const std::string &RequestParser::getVersion() const
{
    return _version;
}

const std::multimap<std::string, std::string> &RequestParser::getHeaders() const
{
    return _headers;
}

const std::string &RequestParser::getBody() const
{
    return _body;
}

void RequestParser::flushBuffer()
{
    _buffer.clear();
}

void RequestParser::reset()
{
    _method.clear();
    _target.clear();
    _version.clear();
    _headers.clear();
    _host.clear();
    _body.clear();
    _buffer.clear();
    _contentLength = 0;
    flushBuffer();
    _state = START_1;
    _isComplete = false;
    _isChunked = false;
    _noBody = true;
}

void RequestParser::parseStartLine(const std::string &line)
{
    size_t pos = line.find(' ');
    if (pos == std::string::npos)
    {
        setErrorState("Invalid request line", 400);
        return;
    }
    _method = line.substr(0, pos);
    if (_method != "GET" && _method != "POST" && _method != "DELETE")
    {
        setErrorState("Method " + _method + " not implemented", 501);
        return;
    }
    size_t pos2 = line.find(' ', pos + 1);
    if (pos2 == std::string::npos)
    {
        setErrorState("Invalid request line", 400);
        return;
    }
    _target = line.substr(pos + 1, pos2 - pos - 1);
    //TODO: validate target (origin-form and absolute-form)
    // -> if absolute-form => use authority (host:port) from request line to set host header
    if (isAbsoluteTarget(_target))
    {}
    else if (!isOriginTarget(_target))
    {
        setErrorState("Invalid target " + _target, 400);
        return;
    }
    if (_target.length() > 8000)
    {
        setErrorState("Request-URI Too Long", 414);
        return;
    }
    _version = line.substr(pos2 + 1);
    if (_version != "HTTP/1.0" && _version != "HTTP/1.1")
    {
        setErrorState("Version " + _version + " not implemented", 505);
        return;
    }
    _state = HEADERS;
    _logger.logDebug("state => HEADERS");
}

void RequestParser::parseChunkedBody(const std::string &line)
{
    (void)line;
}

void RequestParser::parseBody(const std::string &line)
{
    (void)line;

};

void RequestParser::parseHeaders(const std::string &line)
{
    if (line.empty())
    {
        if (!isBodyExpected())
        {
            _isComplete = true;
            _state = COMPLETE;
            _logger.logDebug("state => COMPLETE");
            return;
        }
        else
        {
            _state = BODY;
            _logger.logDebug("state => BODY");
            return;
        }
    }
    size_t pos = line.find(':');
    if (pos == std::string::npos)
    {
        setErrorState("Invalid header line", 400);
        return;
    }
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    if (key.empty() || *(--(key.end())) == ' ' || *(key.begin()) == ' ')
    
    {
        setErrorState("Invalid header line", 400);
        return;
    }
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (!cleanFieldValue(value) || value.empty())
    {
        setErrorState("Invalid header line", 400);
        return;
    }
    if (key == "host")
    {
        if (_host.empty())
            _host = value;
        else
        {
            setErrorState("Host already set", 400);
            return;
        }
    }
    else if (key == "content-length")
    {
        if (!toSizeTNoThrow(value, _contentLength))
        {
            setErrorState("Invalid Content-Length value", 400);
            return;
        }
        if (_contentLength > 8000)
        {
            setErrorState("Content-Length Too Long", 413);
            return;
        }
        if (_contentLength == 0)
            _noBody = true;
        else
            _noBody = false;
    }
    else if (key == "transfer-encoding")
    {
        if (value == "chunked")
            _isChunked = true;
        else
        {
	        setErrorState("Unsupported Transfer-Encoding", 501);
	        return;
        }
    }
    else if (key == "host")
    {
        if (_host.empty())
            _host = value;
        else
        {
            setErrorState("Host already set", 400);
            return;
        }
    }
    else
    {
        _headers.insert(std::make_pair(key, value));
        _logger.logDebug("Adding Header: " + key + ": " + value);
    }
}




