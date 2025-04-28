/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMiddleware.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:29:31 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 15:08:56 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMIDDLEWARE_HPP
#define IMIDDLEWARE_HPP

#include "IRequestHandler.hpp"

class IMiddleware : public IRequestHandler
{
public:
	/**
	 * @brief Destructor
	 * @details Concrete classes should free next handler if they have one. (they own it)
	 */
	virtual ~IMiddleware() {};

	virtual const IRequestHandler *getNext() const = 0;
	virtual void setNext(IRequestHandler *next) = 0;
};

#endif // IMIDDLEWARE_HPP