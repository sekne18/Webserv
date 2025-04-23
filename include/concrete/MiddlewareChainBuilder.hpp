/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiddlewareChainBuilder.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:21:53 by fmol              #+#    #+#             */
/*   Updated: 2025/04/16 14:24:41 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIDDLEWARECHAINBUILDER_HPP
#define MIDDLEWARECHAINBUILDER_HPP

#include <vector>

#include "IMiddleware.hpp"
#include "IRequestHandler.hpp"

class MiddlewareChainBuilder
{
public:
	MiddlewareChainBuilder();
	~MiddlewareChainBuilder();
	MiddlewareChainBuilder(const MiddlewareChainBuilder &other);
	MiddlewareChainBuilder &operator=(const MiddlewareChainBuilder &other);

	MiddlewareChainBuilder &use(IMiddleware *middleware);
	MiddlewareChainBuilder &handle(IRequestHandler *handler);
	IRequestHandler *build();

private:
	std::vector<IMiddleware *> _middlewares;
	IRequestHandler *_baseHandler;
};

#endif // MIDDLEWARECHAINBUILDER_HPP