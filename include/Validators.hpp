/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validators.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:04:56 by fmol              #+#    #+#             */
/*   Updated: 2025/03/12 11:22:54 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATORS_HPP
#define VALIDATORS_HPP

#include <string>
#include <stdexcept>
#include <vector>
#include "Utils.hpp"

const std::vector<std::string> &getValidMethods();
const std::vector<std::string> &getsupportedExtensions();

void validateIp(std::string const &ip);
void validatePort(size_t port);
void validateServerName(std::string const &serverName);
void validatePath(std::string const &path);
void validateMethod(std::string const &method);
void validateReturnCode(size_t code);
void validateMaxSize(size_t size);
bool hasCGIExtension(std::string const &path);


#endif // VALIDATORS_HPP