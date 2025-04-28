/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRequestContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:28:46 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 15:25:43 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREQUESTCONTEXT_HPP
#define IREQUESTCONTEXT_HPP

class IRequestContext
{
public:
    virtual ~IRequestContext() {};

	virtual const std::string &getIp() const = 0;
	virtual size_t getPort() const = 0;
};

#endif // IREQUESTCONTEXT_HPP