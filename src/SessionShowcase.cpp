/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionShowcase.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:33:30 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 15:06:32 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SessionShowcase.hpp"

SessionRequestHandler::SessionRequestHandler()
{
}

SessionRequestHandler::~SessionRequestHandler()
{
}

IResponse *SessionRequestHandler::handle(IRequestParser &request, IHandlerContext *ctx)
{
	if (!ctx)
	{
		return new ConcreteResponse(500, "Internal Server Error");
	}
	if (request.getMethod() == "GET")
	{
		ISession *session = ctx->getSession();
		if (session)
		{
			std::string username = session->get("username");
			if (username.empty())
			{
				session->set("username", "default_user");
			}
			else
			{
				session->set("username", username + "_updated");
			}
			IResponse *response = new ConcreteResponse(200, "OK");
			session->attachSessionCookie(*response);
			response->setBody("<html><body><h1>Session ID: " + session->getId()
				+ "</h1><p>Username: " + session->get("username") + "</p></body></html>");
			response->addHeader("Content-Type", "text/html");
			return response;
		}
		else
		{
			return new ConcreteResponse(404, "Session not found");
		}
	}
	return new ConcreteResponse(405, "Method Not Allowed");
}

void setUpRoutes(IDispatcher *dispatcher)
{
	dispatcher->addRoute("any", 8080, "/session", new SessionRequestHandler());
}