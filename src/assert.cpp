/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:27:52 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:28:19 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "assert.hpp"

void assert(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}