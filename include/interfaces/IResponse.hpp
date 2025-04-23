/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:26:08 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 15:41:55 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <string>

class IResponse
{
  public:
    virtual ~IResponse() {};

    virtual std::string getStatus() const = 0;
    virtual std::string getHeaders() const = 0;

    virtual void fetchData() = 0;
    virtual bool hasAvailableData() const = 0;
    virtual std::string getNextData() const = 0;
    virtual void shiftData(size_t bytes) = 0;
    virtual bool isComplete() const = 0;
};

#endif // IRESPONSE_HPP