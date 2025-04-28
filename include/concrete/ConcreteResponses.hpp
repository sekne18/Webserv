/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteResponses.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:21:08 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:34:11 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETERESPONSES_HPP
#define CONCRETERESPONSES_HPP

#include <string>

#include "IResponse.hpp"
#include "Utils.hpp"

class ConcreteResponse : public IResponse
{
public:
	enum TransferEncoding
	{
		TE_NONE,
		TE_CHUNKED
	};

	ConcreteResponse();
	ConcreteResponse(size_t code, const std::string &message);
    ~ConcreteResponse();

    size_t getStatus() const; //override;
    const std::string &getStatusMessage() const; //override;
    const std::vector<std::pair<std::string, std::string> > &getHeaders() const; //override;
    const std::string &getBody() const; //override;

    void setStatus(size_t status); //override;
    void setStatusMessage(const std::string &statusMessage); //override;
    void addHeader(const std::string &key, const std::string &value); //override;
    void setBody(const std::string &body); //override;
    void appendBody(const std::string &body); //override;
	//void setTransferEncoding(TransferEncoding te);
    
    std::string getStatusLine() const; //override;
    std::string serializeHeaders() const; //override;
    std::string serialize() const; //override;
private:
    size_t _code;
    std::string _message;
    std::vector<std::pair<std::string, std::string> > _headers;
    std::string _body;
	TransferEncoding _transferEncoding;
};

#endif // CONCRETERESPONSES_HPP