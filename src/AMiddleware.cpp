/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMiddleware.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:58:07 by fmol              #+#    #+#             */
/*   Updated: 2025/04/28 09:27:16 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMiddleware.hpp"

AMiddleware::AMiddleware() : _next(0)
{
}

AMiddleware::~AMiddleware()
{
	if (_next)
		delete _next;
}

const IRequestHandler *AMiddleware::getNext() const
{
	return _next;
}

void AMiddleware::setNext(IRequestHandler *next)
{
	if (_next)
		delete _next;
	_next = next;
}