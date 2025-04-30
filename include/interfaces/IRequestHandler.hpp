/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRequestHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:26:02 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:37:15 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "IRequestParser.hpp"
#include "IResponse.hpp"
#include "IHandlerContext.hpp"
#include "ISession.hpp"

class IRequestHandler
{
public:
	virtual ~IRequestHandler() {};

	virtual IResponse *handle(IRequestParser &request, IHandlerContext *ctx) = 0;
};

#endif // IREQUESTHANDLER_HPP