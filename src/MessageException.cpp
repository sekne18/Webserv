/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageException.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:26:34 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 09:26:59 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageException.hpp"

MessageException::MessageException(const std::string &message, int code)
	: message(message), code(code)
{
}

MessageException::~MessageException() throw()
{
}

const char *MessageException::what() const throw()
{
	return message.c_str();
}