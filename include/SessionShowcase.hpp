/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionShowcase.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:31:45 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 15:06:21 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONSHOWCASE_HPP
#define SESSIONSHOWCASE_HPP

#include "Session.hpp"
#include "ConcreteResponses.hpp"
#include "ConcreteRequestHandlers.hpp"
#include "ConcreteMiddleware.hpp"
#include "MiddlewareChainBuilder.hpp"
#include "IDispatcher.hpp"

void setUpRoutes(IDispatcher *dispatcher);

class SessionRequestHandler : public IRequestHandler
{
public:
	SessionRequestHandler();
	~SessionRequestHandler();

	IResponse *handle(IRequestParser &request, IHandlerContext *ctx); //override;
};

	

#endif // SESSIONSHOWCASE_HPP