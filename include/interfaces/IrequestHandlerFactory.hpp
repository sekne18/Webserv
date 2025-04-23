/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrequestHandlerFactory.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:11:08 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:43:23 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREQUESTHANDLERFACTORY_HPP
#define IREQUESTHANDLERFACTORY_HPP

#include "IHandlerContext.hpp"
#include "IRequestHandler.hpp"

class IRequestHandlerFactory
{
  public:
    virtual ~IRequestHandlerFactory() {};

    virtual IRequestHandler *createHandler(IHandlerContext &context) = 0;
};

#endif // IREQUESTHANDLERFACTORY_HPP