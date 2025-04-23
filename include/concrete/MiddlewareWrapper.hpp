/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiddlewareWrapper.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:25:14 by fmol              #+#    #+#             */
/*   Updated: 2025/04/16 14:27:12 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIDDLEWAREWRAPPER_HPP
#define MIDDLEWAREWRAPPER_HPP

#include "IRequestHandler.hpp"
#include "IMiddleware.hpp"

class MiddlewareWrapper : public IRequestHandler
{
public:
	MiddlewareWrapper(IMiddleware *m, IRequestHandler *next);
	~MiddlewareWrapper();

	IResponse *handle(const IRequestParser &request, IHandlerContext &ctx); // override
};

#endif // MIDDLEWAREWRAPPER_HPP