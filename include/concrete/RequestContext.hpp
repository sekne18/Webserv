/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:49:31 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:50:15 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONTEXT_HPP
#define REQUESTCONTEXT_HPP

#include "IRequestContext.hpp"

class RequestContext : public IRequestContext
{
  public:
    RequestContext();
    ~RequestContext();
};

#endif // REQUESTCONTEXT_HPP