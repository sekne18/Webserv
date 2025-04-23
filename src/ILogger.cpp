/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ILogger.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:02:13 by fmol              #+#    #+#             */
/*   Updated: 2025/04/22 09:06:18 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ILogger.hpp"

ILogger::LogLevel operator|(ILogger::LogLevel lhs, ILogger::LogLevel rhs)
{
    return static_cast<ILogger::LogLevel>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

ILogger::LogLevel operator|=(ILogger::LogLevel &lhs, ILogger::LogLevel rhs)
{
    lhs = lhs | rhs;
    return lhs;
}