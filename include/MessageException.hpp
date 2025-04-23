/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageException.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:22:43 by fmol              #+#    #+#             */
/*   Updated: 2025/04/23 10:06:03 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEEXCEPTION_HPP
#define MESSAGEEXCEPTION_HPP

#include <exception>
#include <string>

class MessageException : public std::exception
{
public:
	MessageException(const std::string &message, int code);
	~MessageException() throw();
	const char *what() const throw();

	const std::string message;
	int code;
};
#endif // MESSAGEEXCEPTION_HPP