/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISession.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:20:59 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:43:10 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISESSION_HPP
#define ISESSION_HPP

#include <string>

class ISession
{
  public:
    virtual ~ISession() {};

    virtual std::string getId() const = 0;
    virtual std::string get(const std::string &key) const = 0;
    virtual void set(const std::string &key, const std::string &value) = 0;
    virtual void remove(const std::string &key) = 0;
};

#endif // ISESSION_HPP