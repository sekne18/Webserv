/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:18:18 by fmol              #+#    #+#             */
/*   Updated: 2025/03/11 15:34:19 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "Utils.hpp"

namespace TokenType
{
	enum Type
	{
		UNKNOWN = 0,
		SERVER = 1,
		LOCATION,
		ROOT,
		LISTEN,
		SERVER_NAME,
		INDEX,
		AUTO,
		OFF,
		ERROR_PAGE,
		MAX_SIZE,
		ALLOW,
		RETURN,
		L_BRACE,
		R_BRACE,
		COLON,
		SEMICOLON,
		DOMAIN_VALUE,
		PATH,
		IP_VALUE,
		NUMBER,
		REQUEST,
		END
	};
};

const std::map<std::string, TokenType::Type> &getKeywords();

struct Token
{
	TokenType::Type type;
	std::string value;
	size_t line;
	size_t column;
};

class Lexer
{
public:
	Lexer(const std::string &input);
	Lexer(std::ifstream &file);
	Lexer(const Lexer &lexer);
	Lexer &operator=(const Lexer &lexer);
	~Lexer();
	Token getNextToken();
private:
	char peek(size_t n);
	char advance();
	void skipWhitespace();
	void skipComment();
	bool setDirective(Token &token);
	bool matchSpecialChar(Token &token);
	bool matchDirective(Token &token);
	bool matchDomain(Token &token);
	bool matchPath(Token &token);
	bool matchDigit(Token &token);

	std::string _input;
	size_t _pos;
	size_t _line;
	size_t _column;
};

#endif // LEXER_HPP