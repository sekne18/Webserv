/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:32:18 by fmol              #+#    #+#             */
/*   Updated: 2025/03/15 16:55:33 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

size_t Utils::toSizeT(std::string const &str)
{
	size_t val;
	std::istringstream iss(str);
	iss >> val;
	return (val);
}

bool Utils::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool Utils::isSpecial(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == ':');
}

bool Utils::isMark(char c)
{
	return (c == '-' || c == '_' || c == '.' || c == '!' || c == '~' || c == '*' || c == '\'' || c == '(' || c == ')');
}

bool Utils::isUnreserved(char c)
{
	return (isAlphaNum(c) || isMark(c));
}

bool Utils::isReserved(char c)
{
	return (c == '$' || c == '&' || c == '+' || c == ',' || c == '/' || c == ':' || c == ';' || c == '=' || c == '?' || c == '@');
}

bool Utils::isHex(char c)
{
	return (isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool Utils::isLowAlpha(char c)
{
	return (c >= 'a' && c <= 'z');
}

bool Utils::isUpAlpha(char c)
{
	return (c >= 'A' && c <= 'Z');
}

bool Utils::isAlpha(char c)
{
	return (isLowAlpha(c) || isUpAlpha(c));
}

bool Utils::isIdentifier(char c)
{
	return (isAlpha(c) || isDigit(c) || c == '_' || c == '-');
}

bool Utils::isDomain(char c)
{
	return (isAlphaNum(c) || c == '.' || c == '-' || c == '*');
}

bool Utils::isDirective(char c)
{
	return (isAlpha(c) || c == '_');
}

bool Utils::isPath(char c)
{
	return (isIdentifier(c) || c == '/' || c == '*' || c == '.');
}

bool Utils::isIp(char c)
{
	return (isDigit(c) || c == '.');
}

bool Utils::isAlphaNum(char c)
{
	return (isAlpha(c) || isDigit(c));
}

bool Utils::isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool Utils::contains(const std::vector<std::string>& vec, const std::string& value) {
    return (std::find(vec.begin(), vec.end(), value) != vec.end());
}

int Utils::stringToInt(const std::string& str)
{ 
    // Convert to long int, specify base 10 (decimal)
    return static_cast<int>(std::strtol(str.c_str(), NULL, 10));
}

