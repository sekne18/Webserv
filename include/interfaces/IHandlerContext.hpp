/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHandlerContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:12:52 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 13:49:34 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHANDLERCONTEXT_HPP
#define IHANDLERCONTEXT_HPP

#include "ISession.hpp"

class IHandlerContext
{
  public:
    virtual ~IHandlerContext() {};
    virtual ISession *getSession() const = 0;
};

#endif // IHANDLERCONTEXT_HPP