/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IResponseWriter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:11:44 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 08:14:55 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRESPONSEWRITER_HPP
#define IRESPONSEWRITER_HPP

#include <string>

#include "IResponse.hpp"

class IResponseWriter
{
  public:
	virtual ~IResponseWriter() {};

	virtual void start(const IResponse &response) = 0;
	virtual std::string getNextData(size_t bytes) = 0;
	virtual void advanceData(size_t bytes) = 0;
	virtual bool isComplete() const = 0;

};

#endif // IRESPONSEWRITER_HPP