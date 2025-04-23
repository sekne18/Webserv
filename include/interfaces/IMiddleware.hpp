/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMiddleware.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:17:11 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:43:36 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMIDDLEWARE_HPP
#define IMIDDLEWARE_HPP

#include "IHandlerContext.hpp"
#include "IRequestHandler.hpp"
#include "IRequestParser.hpp"
#include "IResponse.hpp"

class IMiddleware
{
  public:
    virtual ~IMiddleware() {};

    virtual IResponse *handle(const IRequestParser &request, IHandlerContext &ctx, IRequestHandler *next) = 0;
};

#endif // IMIDDLEWARE_HPP