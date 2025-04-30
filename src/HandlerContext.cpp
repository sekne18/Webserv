/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:03:07 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 13:49:24 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HandlerContext.hpp"

HandlerContext::HandlerContext(ISession *session)
	: IHandlerContext(), isDir(false), _session(session)
{
}

HandlerContext::~HandlerContext()
{
}

ISession *HandlerContext::getSession() const
{
	return _session;
}