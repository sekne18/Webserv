/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:26:08 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:29:22 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <string>
#include <vector>

class IResponse
{
public:
    virtual ~IResponse() {};

    virtual size_t getStatus() const = 0;
    virtual const std::string &getStatusMessage() const = 0;
    virtual const std::vector<std::pair<std::string, std::string> > &getHeaders() const = 0;
    virtual const std::string &getBody() const = 0;

    virtual void setStatus(size_t status) = 0;
    virtual void setStatusMessage(const std::string &statusMessage) = 0;
    virtual void addHeader(const std::string &key, const std::string &value) = 0;
    virtual void setBody(const std::string &body) = 0;
    virtual void appendBody(const std::string &body) = 0;

    virtual std::string getStatusLine() const = 0;
    virtual std::string serializeHeaders() const = 0;
	virtual std::string serialize() const = 0;
  

};

#endif // IRESPONSE_HPP