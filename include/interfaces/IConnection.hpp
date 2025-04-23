/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IConnection.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:14:31 by fmol              #+#    #+#             */
/*   Updated: 2025/04/18 16:43:49 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICONNECTION_HPP
#define ICONNECTION_HPP

class IConnection
{
  public:
    virtual ~IConnection() {};

    virtual void onReadable() = 0;
    virtual void onWritable() = 0;
};

#endif // ICONNECTION_HPP