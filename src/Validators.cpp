/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validators.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:05:41 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 15:54:59 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validators.hpp"

const std::vector<std::string> &getValidMethods()
{
	static std::vector<std::string> validMethods;
	if (validMethods.empty())
	{
		validMethods.push_back("GET");
		validMethods.push_back("POST");
		validMethods.push_back("DELETE");
	}
	return (validMethods);
}

const std::vector<std::string> &getsupportedExtensions()
{
	static std::vector<std::string> supportedExtensions;
	if (supportedExtensions.empty())
	{
		supportedExtensions.push_back("py");
		supportedExtensions.push_back("php");
		supportedExtensions.push_back("sh");
	}
	return (supportedExtensions);
}

void validateIp(std::string const &ip)
{
	std::istringstream iss(ip);
	int a, b, c, d;
	char dot;
	iss >> a >> dot >> b >> dot >> c >> dot >> d;
	if (iss.fail()
		|| a < 0 || a > 255
		|| b < 0 || b > 255
		|| c < 0 || c > 255
		|| d < 0 || d > 255)
		throw std::runtime_error("Invalid IP address: " + ip);
}

void validatePort(size_t port)
{
	if (port < 1 || port > 65535)
		throw std::runtime_error("Invalid port number: " + toString(port));
}

void validateServerName(std::string const &serverName)
{
	if (serverName.size() > 255)
		throw std::runtime_error("Server name too long: " + serverName);
	if (serverName[0] == '-' || serverName[serverName.size() - 1] == '-')
		throw std::runtime_error("Server name must not start or end with a hyphen: " + serverName);
	std::string tmp(serverName);
	std::string label;
	while (serverName.size() > 0)
	{
		size_t pos = tmp.find('.');
		if (pos == std::string::npos)
			pos = tmp.size();
		label = tmp.substr(0, pos);
		if (label.size() > 63)
			throw std::runtime_error("Server name label too long: " + label);
		else if (label.size() < 1)
			throw std::runtime_error("Server name label too short: " + label);
		for (size_t i = 1; i < label.size() - 1; ++i)
		{
			if (!isAlphaNum(label[i]) && label[i] != '-')
				throw std::runtime_error("Server name label must contain only letters, digits, and hyphens: " + label);
		}
		if (pos >= tmp.size())
			break;
		tmp = tmp.substr(pos + 1);
	}
}

void validatePath(std::string const &path)
{
	if (path.size() > 1024)
		throw std::runtime_error("Path too long: " + path);
	size_t pos1 = path.find_last_of("*");
	size_t pos2 = path.find_last_of("/");
	if (pos1 != std::string::npos && pos2 != std::string::npos && pos1 < pos2)
		throw std::runtime_error("Invalid path: " + path);
	if (path.find("*.") != std::string::npos)
	{
		std::string extension = path.substr(path.find("*.") + 2);
		if (extension.size() > 63)
			throw std::runtime_error("Extension too long: " + extension);
		if (!contains(getsupportedExtensions(), extension))
			throw std::runtime_error("Unsupported extension: " + extension);
	}
	if (path[0] != '/')
		throw std::runtime_error("Path must start with a '/', end with \"*.cgiExtension\" in case of cgi: " + path);
}

void validateMethod(std::string const &method)
{
	if (!contains(getValidMethods(), method))
		throw std::runtime_error("Unsupported method: " + method);
}

void validateReturnCode(size_t code)
{
	if (code < 100 || code > 599)
		throw std::runtime_error("Invalid return code: " + toString(code));
}

void validateMaxSize(size_t size)
{
	if (size < 1)
		throw std::runtime_error("Invalid max_size: " + toString(size));
}

bool hasCGIExtension(std::string const &path)
{
	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos || pos + 1 >= path.size())
		return (false);
	std::string extension = path.substr(pos + 1);
	return (contains(getsupportedExtensions(), extension));
}

bool isAbsoluteTarget(std::string const &target)
{
	if (target.size() < 9)
		return (false);
	if (target.find("http://") == 0)
		return (true);
	return (false);
}

bool isOriginTarget(std::string const &target)
{
	if (target.empty())
		return (false);
	if (target[0] == '/')
		return (true);
	return (false);
}