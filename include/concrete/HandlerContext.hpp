/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:01:39 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 11:02:38 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLERCONTEXT_HPP
#define HANDLERCONTEXT_HPP

#include "IHandlerContext.hpp"

class HandlerContext : public IHandlerContext
{
public:
	HandlerContext();
	~HandlerContext();

	bool isDir;
};

#endif // HANDLERCONTEXT_HPP