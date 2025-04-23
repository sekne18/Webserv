/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IDispatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:23:52 by fmol              #+#    #+#             */
/*   Updated: 2025/04/17 13:21:27 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDISPATCHER_HPP
#define IDISPATCHER_HPP

#include "IRequestParser.hpp"
#include "IResponse.hpp"
#include "IRequestHandler.hpp"
#include "IRequestContext.hpp"

class IDispatcher
{
public:
	virtual ~IDispatcher() {};

	virtual IResponse *dispatch(const IRequestParser *request, IRequestContext &ctx) = 0;
	// virtual void addRoute(const std::string &path, IRequestHandler *handler) = 0;
	// virtual void registerHandler(const std::string &method, const std::string &path, IRequestHandler *handler) = 0; // TODO: WIP
};

#endif // IDISPATCHER_HPP