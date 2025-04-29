/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRequestParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:20:41 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 09:24:19 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREQUESTPARSER_HPP
#define IREQUESTPARSER_HPP

#include <string>
#include <map>

class IRequestParser
{
public:
	virtual ~IRequestParser() {};

	virtual void parse(const std::string &data) = 0;
	virtual bool isComplete() const = 0;
	virtual bool isErroneous() const = 0;
	virtual const std::string &getErrorMessage() const = 0;
	virtual size_t getErrorCode() const = 0;
	virtual const std::string &getMethod() const = 0;
	virtual const std::string &getTarget() const = 0;
	virtual const std::string &getVersion() const = 0;
	virtual const std::string &getHost() const = 0;
	virtual const std::multimap<std::string, std::string> &getHeaders() const = 0;
	virtual const std::string &getBody() const = 0;

	virtual void setTarget(const std::string &target) = 0;
	
	virtual void reset() = 0;
};

#endif // IREQUESTPARSER_HPP