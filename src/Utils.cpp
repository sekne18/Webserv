/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:32:18 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 15:47:30 by fmol             ###   ########.fr       */
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

bool isMatchInterface(std::string const &ip1, size_t port1,
						 std::string const &ip2, size_t port2)
{
	return ((ip1 == ip2 || ip1 == "any" || ip2 == "any") && port1 == port2);
}

std::string *loadFile(std::string const &path)
{
	if (path.empty())
		return (0);
	std::string dupPath(path);
	if (dupPath[0] == '/')
		dupPath.erase(0, 1);
	std::ifstream file(dupPath.c_str());
	if (!file.is_open())
		return (0);
	std::string *content = new std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return (content);
}

bool isDirectory(std::string const &path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			return true;
	}
	else
	{
		std::string errMsg = std::strerror(errno);
		if (errno == ENOENT || errno == ENOTDIR || errno == ENAMETOOLONG)
			return false;
		else if (errno == EACCES)
			throw std::runtime_error("Permission denied: " + path);
		else
			throw std::runtime_error("Error checking path: " + path + ": " + errMsg);
	}
	return false;
}

std::string generateDirectoryListing(std::string const &path)
{
    std::string listing = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    listing += "<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    listing += "<title>Directory Listing</title>\n";
    listing += "<style>\n";
    listing += "body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; padding: 20px; }\n";
    listing += "h1 { font-size: 24px; }\n";
    listing += "ul { list-style-type: none; padding: 0; }\n";
    listing += "li { margin: 5px 0; }\n";
    listing += "a { color: #3498db; text-decoration: none; }\n";
    listing += "a:hover { text-decoration: underline; }\n";
    listing += "</style>\n</head>\n<body>\n";
    listing += "<h1>Directory Listing for " + path + "</h1>\n<ul>\n";

    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
        throw std::runtime_error("Failed to open directory: " + path + ": " + std::string(std::strerror(errno)));

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            std::string fileName = entry->d_name;
            listing += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>\n";
        }
    }
    closedir(dir);
    listing += "</ul>\n</body>\n</html>";
    return listing;
}

const std::string &getGenericStatusMessage(size_t statusCode)
{
	static std::map<size_t, std::string> statusMessages;
	if (statusMessages.empty())
	{
		statusMessages[0] = "Unknown Status";
		statusMessages[200] = "OK";
		statusMessages[201] = "Created";
		statusMessages[204] = "No Content";
		statusMessages[301] = "Moved Permanently";
		statusMessages[302] = "Found";
		statusMessages[303] = "See Other";
		statusMessages[304] = "Not Modified";
		statusMessages[307] = "Temporary Redirect";
		statusMessages[308] = "Permanent Redirect";
		statusMessages[400] = "Bad Request";
		statusMessages[401] = "Unauthorized";
		statusMessages[403] = "Forbidden";
		statusMessages[404] = "Not Found";
		statusMessages[500] = "Internal Server Error";
		statusMessages[501] = "Not Implemented";
	}

	std::map<size_t, std::string>::iterator it = statusMessages.find(statusCode);
	if (it != statusMessages.end())
		return it->second;
	return statusMessages[0];
}