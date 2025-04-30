/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionIdGenerator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:05:56 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 15:06:03 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SessionIdGenerator.hpp"

SessionIdGenerator::SessionIdGenerator(size_t bufferSize) : _bufferSize(bufferSize), _bufferIndex(_bufferSize + 1),
															_buffer(new unsigned char[bufferSize])
{
	//TODO: check if bufferSize is within reasonable limits
	if (bufferSize > 1024 * 1024)
		throw std::runtime_error("Buffer size too large");
	if (_buffer == 0)
		throw std::runtime_error("Failed to allocate memory for buffer");
	if (!refreshBuffer())
		throw std::runtime_error("Failed to read from /dev/urandom");
}

SessionIdGenerator::~SessionIdGenerator()
{
	delete[] _buffer;
}

bool SessionIdGenerator::refreshBuffer()
{
	std::ifstream urandom("/dev/urandom", std::ios::binary | std::ios::in);
	if (!urandom)
		return false;
	urandom.read(reinterpret_cast<char *>(_buffer), _bufferSize);
	return (urandom.gcount() == static_cast<std::streamsize>(_bufferSize));
}

bool SessionIdGenerator::getRandomBytes(unsigned char *dest, size_t size)
{
	size_t bytes_read = 0;
	while (bytes_read < size)
	{
		if (_bufferIndex >= _bufferSize)
		{
			if (!refreshBuffer())
				return (false);
			_bufferIndex = 0;
		}
		size_t bytes_to_copy = std::min(size - bytes_read, _bufferSize - _bufferIndex);
		std::copy(_buffer + _bufferIndex, _buffer + _bufferIndex + bytes_to_copy, dest + bytes_read);
		bytes_read += bytes_to_copy;
		_bufferIndex += bytes_to_copy;
	}
	return (true);
}

std::string SessionIdGenerator::generateSessionId(size_t length)
{
	std::vector<unsigned char> randomBytes(length / 2);
	if (!getRandomBytes(randomBytes.data(), length / 2))
		throw std::runtime_error("Failed to generate session ID");
	std::ostringstream oss;
	for (size_t i = 0; i < length / 2; ++i)
	{
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(randomBytes[i]);
	}
	return (oss.str());
}

std::string SessionIdGenerator::generateUniqueSessionId(std::set<std::string> &existingIds, size_t length)
{
	std::string sessionId;
	do
	{
		sessionId = generateSessionId(length);
	} while (existingIds.find(sessionId) != existingIds.end());
	return (sessionId);
}