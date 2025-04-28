/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiddlewareChainBuilder.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:18:42 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 11:35:54 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiddlewareChainBuilder.hpp"

MiddlewareChainBuilder::MiddlewareChainBuilder()
	: _baseHandler(0)
{}

MiddlewareChainBuilder::~MiddlewareChainBuilder()
{}

MiddlewareChainBuilder &MiddlewareChainBuilder::use(IMiddleware *middleware)
{
	_middlewares.push_back(middleware);
	return *this;
}

MiddlewareChainBuilder &MiddlewareChainBuilder::handle(IRequestHandler *handler)
{
	_baseHandler = handler;
	return *this;
}

IRequestHandler *MiddlewareChainBuilder::build()
{
	IRequestHandler *handler = _baseHandler;
	for (std::vector<IMiddleware *>::reverse_iterator it = _middlewares.rbegin(); it != _middlewares.rend(); ++it)
	{
		(*it)->setNext(handler);
		handler = *it;
	}
	return handler;
}

void MiddlewareChainBuilder::clear()
{
	_middlewares.clear();
	_baseHandler = 0;
}