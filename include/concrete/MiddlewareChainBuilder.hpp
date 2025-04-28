/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiddlewareChainBuilder.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:21:53 by fmol              #+#    #+#             */
/*   Updated: 2025/04/24 14:11:07 by fmol             ###   ########.fr       */
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

	MiddlewareChainBuilder &use(IMiddleware *middleware);
	MiddlewareChainBuilder &handle(IRequestHandler *handler);
	IRequestHandler *build();
	void clear();

private:
	std::vector<IMiddleware *> _middlewares;
	IRequestHandler *_baseHandler;
};

#endif // MIDDLEWARECHAINBUILDER_HPP