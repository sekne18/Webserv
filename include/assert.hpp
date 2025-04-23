/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:16:36 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:27:44 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <iostream>
#include <stdexcept>

void assert(bool condition, const std::string &message);

#endif // ASSERT_HPP