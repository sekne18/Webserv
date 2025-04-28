/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMiddleware.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:56:40 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 13:02:39 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMIDDLEWARE_HPP
#define AMIDDLEWARE_HPP

#include "IMiddleware.hpp"

class AMiddleware : public IMiddleware
{
public:
	virtual ~AMiddleware();

	const IRequestHandler *getNext() const; // override
	void setNext(IRequestHandler *next); // override
protected:
	AMiddleware();
	IRequestHandler *_next; // owned
};

#endif // AMIDDLEWARE_HPP