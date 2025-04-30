/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:12:21 by fmol              #+#    #+#             */
/*   Updated: 2025/04/30 14:26:57 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <map>

#include "ISession.hpp"

class Session : public ISession
{
public:
	Session(const std::string &id);
	~Session();

	std::string getId() const; //override;
	std::string get(const std::string &key) const; //override;
	void set(const std::string &key, const std::string &value); //override;
	void remove(const std::string &key); //override;
	void attachSessionCookie(IResponse &response); // override;
private:
	std::string _id;
	std::map<std::string, std::string> _data;
};

#endif // SESSION_HPP