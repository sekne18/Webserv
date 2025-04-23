/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:32:18 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 14:08:23 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

size_t toSizeT(std::string const &str)
{
	if (str.empty() || str[0] == '-')
	{
		throw std::runtime_error("Invalid size_t value: " + str);
		return (0);
	}
	size_t val = 0;
	std::istringstream iss(str);
	iss >> val;
	if (iss.fail() || !iss.eof())
		throw std::runtime_error("Invalid size_t value: " + str);
	return (val);
}

bool toSizeTNoThrow(std::string const &str, size_t &val)
{
	if (str.empty() || str[0] == '-')
	{
		return false;
	}
	std::istringstream iss(str);
	iss >> val;
	if (iss.fail() || !iss.eof())
		return false;
	return true;
}



bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool isSpecial(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == ':');
}

bool isMark(char c)
{
	return (c == '-' || c == '_' || c == '.' || c == '!' || c == '~' || c == '*' || c == '\'' || c == '(' || c == ')');
}

bool isUnreserved(char c)
{
	return (isAlphaNum(c) || isMark(c));
}

bool isReserved(char c)
{
	return (c == '$' || c == '&' || c == '+' || c == ',' || c == '/' || c == ':' || c == ';' || c == '=' || c == '?' || c == '@');
}

bool isHex(char c)
{
	return (isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool isLowAlpha(char c)
{
	return (c >= 'a' && c <= 'z');
}

bool isUpAlpha(char c)
{
	return (c >= 'A' && c <= 'Z');
}

bool isAlpha(char c)
{
	return (isLowAlpha(c) || isUpAlpha(c));
}

bool isIdentifier(char c)
{
	return (isAlpha(c) || isDigit(c) || c == '_' || c == '-');
}

bool isDomain(char c)
{
	return (isAlphaNum(c) || c == '.' || c == '-' || c == '*');
}

bool isDirective(char c)
{
	return (isAlpha(c) || c == '_');
}

bool isPath(char c)
{
	return (isIdentifier(c) || c == '/' || c == '*' || c == '.');
}

bool isIp(char c)
{
	return (isDigit(c) || c == '.');
}

bool isAlphaNum(char c)
{
	return (isAlpha(c) || isDigit(c));
}

bool isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool contains(const std::vector<std::string>& vec, const std::string& value) {
    return (std::find(vec.begin(), vec.end(), value) != vec.end());
}

bool cleanFieldValue(std::string &fieldValue)
{
	std::string::iterator it = fieldValue.begin();
	while (it != fieldValue.end())
	{
		if (isWhitespace(*it))
			it = fieldValue.erase(it);
		else
			break;
	}
	it = fieldValue.end();
	--it;
	while (it != fieldValue.begin())
	{
		if (isWhitespace(*it))
			it = fieldValue.erase(it);
		else
			break;
	}
	return true;
}