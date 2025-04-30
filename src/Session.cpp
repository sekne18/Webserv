/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:14:03 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:28:30 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

Session::Session(const std::string &id)
	: _id(id)
{
}

Session::~Session()
{
}

std::string Session::getId() const
{
	return _id;
}

std::string Session::get(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _data.find(key);
	if (it != _data.end())
		return it->second;
	return "";
}

void Session::set(const std::string &key, const std::string &value)
{
	_data[key] = value;
}

void Session::remove(const std::string &key)
{
	_data.erase(key);
}

void Session::attachSessionCookie(IResponse &response)
{
	response.addHeader("Set-Cookie", "session_id=" + _id + "; HttpOnly; Path=/");
}

