/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:32:59 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:24:58 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <sys/types.h>

typedef struct socketInfo
{
    int fd;
    std::string ip;
    size_t port;
} t_socketInfo;

bool operator==(const t_socketInfo &lhs, const int &rhs);

#endif // STRUCTS_HPP