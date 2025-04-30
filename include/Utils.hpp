/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:58:55 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 08:41:45 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <unistd.h>

#include "Validators.hpp"

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss << val;
    return (oss.str());
}

std::string getPwd(char **envp);
std::string combinePaths(std::string const path1, std::string const path2);
bool isCGIMatch(std::string const &path1, std::string const &path2);
bool hexToDec(std::string const &hex, size_t &dec);

std::string *loadFile(std::string const &path);
bool isDirectory(std::string const &path);
bool isFile(std::string const &path);
bool fileExists(std::string const &path);
std::string generateDirectoryListing(std::string const &path);
const std::string &getGenericStatusMessage(size_t statusCode);

size_t toSizeT(std::string const &str);
bool toSizeTNoThrow(std::string const &str, size_t &val);

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

bool isMatchInterface(std::string const &ip1, size_t port1,
                      std::string const &ip2, size_t port2);

bool contains(std::vector<std::string> const &vec, std::string const &str);

bool cleanFieldValue(std::string &fieldValue);

#endif // UTILS_HPP