/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:58:55 by fmol              #+#    #+#             */
/*   Updated: 2025/03/15 16:54:56 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

namespace Utils
{
	int stringToInt(const std::string& str);

	template <typename T>
	std::string toString(T val)
	{
		std::ostringstream oss;
		oss << val;
		return (oss.str());
	}

	size_t toSizeT(std::string const &str);

	bool isDigit(char c);
	bool isSpecial(char c);
	bool isMark(char c);
	bool isUnreserved(char c);
	bool isReserved(char c);
	bool isHex(char c);
	bool isLowAlpha(char c);
	bool isUpAlpha(char c);
	bool isAlpha(char c);
	bool isIdentifier(char c);
	bool isDirective(char c);
	bool isDomain(char c);
	bool isPath(char c);
	bool isIp(char c);
	bool isAlphaNum(char c);
	bool isWhitespace(char c);

	bool contains(std::vector<std::string> const &vec, std::string const &str);
}

#endif // UTILS_HPP