/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseWriter.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:05:02 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 09:07:45 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEWRITER_HPP
#define RESPONSEWRITER_HPP

#include "IResponseWriter.hpp"

class ResponseWriter : public IResponseWriter
{
public:
	ResponseWriter();
	~ResponseWriter();

	void start(const IResponse &response); //override;
	std::string getNextData(size_t bytes); //override;
	void advanceData(size_t bytes); //override;
	bool isComplete() const; //override;

private:
	std::string _data;
	size_t _bytesSent;
	size_t _totalBytes;
	bool _isComplete;
};

#endif // RESPONSEWRITER_HPP