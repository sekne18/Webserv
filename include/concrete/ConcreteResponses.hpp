/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteResponses.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:21:08 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 15:54:37 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETERESPONSES_HPP
#define CONCRETERESPONSES_HPP

#include <string>

#include "IResponse.hpp"
#include "Utils.hpp"

class ErrorResponse : public IResponse
{
  public:
    ErrorResponse(size_t code, const std::string &message);
    ~ErrorResponse();

    std::string getStatus() const;  // override;
    std::string getHeaders() const; // override;

    void fetchData(); // override;
    bool hasAvailableData() const;  // override;
    std::string getNextData() const; // override;
    void shiftData(size_t bytes); // override;
    bool isComplete() const;   // override;
  private:
    size_t _code;
    std::string _message;
    size_t _read;
};

#endif // CONCRETERESPONSES_HPP