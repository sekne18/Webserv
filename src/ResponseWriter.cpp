/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseWriter.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:06:44 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:09:51 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseWriter.hpp"

ResponseWriter::ResponseWriter() 
	: _bytesSent(0),
	  _totalBytes(0),
	  _isComplete(true)
{
}

ResponseWriter::~ResponseWriter()
{
}

void ResponseWriter::start(const IResponse &response)
{
	_data = response.serialize();
	_bytesSent = 0;
	_totalBytes = _data.size();
	_isComplete = false;
}

std::string ResponseWriter::getNextData(size_t bytes)
{
	if (_isComplete)
		return "";
	if (_bytesSent + bytes > _totalBytes)
		bytes = _totalBytes - _bytesSent;
	std::string data = _data.substr(_bytesSent, bytes);
	return data;
}

void ResponseWriter::advanceData(size_t bytes)
{
	_bytesSent += bytes;
	if (_bytesSent == _totalBytes)
		_isComplete = true;
}

bool ResponseWriter::isComplete() const
{
	return _isComplete;
}