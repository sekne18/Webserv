/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:59:32 by fmol              #+#    #+#             */
/*   Updated: 2025/03/11 15:23:08 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

const std::map<std::string, TokenType::Type> &getKeywords()
{
	static std::map<std::string, TokenType::Type> keywords;
	if (keywords.empty())
	{
		keywords["server"] = TokenType::SERVER;
		keywords["location"] = TokenType::LOCATION;
		keywords["root"] = TokenType::ROOT;
		keywords["return"] = TokenType::RETURN;
		keywords["allow"] = TokenType::ALLOW;
		keywords["listen"] = TokenType::LISTEN;
		keywords["server_name"] = TokenType::SERVER_NAME;
		keywords["index"] = TokenType::INDEX;
		keywords["auto"] = TokenType::AUTO;
		keywords["off"] = TokenType::OFF;
		keywords["error_page"] = TokenType::ERROR_PAGE;
		keywords["max_size"] = TokenType::MAX_SIZE;
		keywords["GET"] = TokenType::REQUEST;
		keywords["POST"] = TokenType::REQUEST;
		keywords["DELETE"] = TokenType::REQUEST;
	}
	return (keywords);
};

Lexer::Lexer(std::string const &input) : _input(input), _pos(0), _line(1), _column(1)
{
}

Lexer::Lexer(std::ifstream &file) : _pos(0), _line(1), _column(1)
{
	std::string line;
	while (std::getline(file, line))
		_input += line + '\n';
}

Lexer::Lexer(Lexer const &src)
{
	*this = src;
}

Lexer &Lexer::operator=(Lexer const &rhs)
{
	if (this != &rhs)
	{
		_input = rhs._input;
		_pos = rhs._pos;
		_line = rhs._line;
		_column = rhs._column;
	}
	return (*this);
}

Lexer::~Lexer()
{
}

char Lexer::peek(size_t n = 0)
{
	if (_input.length() <= _pos + n)
		return '\0';
	return (_input[_pos + n]);
}

char Lexer::advance()
{
	if (_input.length() <= _pos)
		return '\0';
	char c = _input[_pos];
	++_pos;
	if (c == '\n')
	{
		++_line;
		_column = 1;
	}
	else
		++_column;
	return (c);
}

void Lexer::skipWhitespace()
{
	while (isWhitespace(peek()))
		advance();
}

void Lexer::skipComment()
{
	while (peek() == '#')
	{
		while (peek() != '\n')
			advance();
		skipWhitespace();
	}
}

bool Lexer::setDirective(Token &token)
{
	std::map<std::string, TokenType::Type>::const_iterator it = getKeywords().find(token.value);
	if (it != getKeywords().end())
	{
		token.type = it->second;
		return (true);
	}
	return (false);
}

bool Lexer::matchSpecialChar(Token &token)
{
	switch (peek())
	{
	case '{':
		token.type = TokenType::L_BRACE;
		token.value = advance();
		return (true);
	case '}':
		token.type = TokenType::R_BRACE;
		token.value = advance();
		return (true);
	case ':':
		token.type = TokenType::COLON;
		token.value = advance();
		return (true);
	case ';':
		token.type = TokenType::SEMICOLON;
		token.value = advance();
		return (true);
	case '\0':
		token.type = TokenType::END;
		token.value = advance();
		return (true);
	default:
		break;
	}
	return (false);
}

bool Lexer::matchPath(Token &token)
{
	char c = peek();
	if (c != '/' && c != '*')
		return (false);
	token.value += advance();
	while (isPath(peek()))
		token.value += advance();
	c = peek();
	if (c != ';' && c != '{' && !isWhitespace(c))
		return (false);
	token.type = TokenType::PATH;
	return (true);
}

bool Lexer::matchDirective(Token &token)
{
	while (isDirective(peek()))
		token.value += advance();
	char c = peek();
	if (!isWhitespace(c) && c != '{')
		return (false);
	if (setDirective(token))
		return (true);
	return (false);
}

bool Lexer::matchDomain(Token &token)
{
	if (token.value.find('/') != std::string::npos)
		return (false);
	if (isDomain(peek()))
	{
		token.value += advance();
		while (isDomain(peek()))
			token.value += advance();
		char c = peek();
		if (c != ';' && !isWhitespace(c))
			return (false);
		token.type = TokenType::DOMAIN_VALUE;
		return (true);
	}
	return (false);
}

bool Lexer::matchDigit(Token &token)
{
	if (!token.value.empty())
		return (false);
	if (isDigit(peek()))
	{
		token.value += advance();
		while (isDigit(peek()))
			token.value += advance();
		char c = peek();
		if (c == '.')
		{
			token.value += advance();
			while (isIp(peek()))
				token.value += advance();
			c = peek();
			if (c != ';' && c != ':')
				return (false);
			token.type = TokenType::IP_VALUE;
			return (true);
		}
		else if (!(isWhitespace(c) || c == ';' || c == '\0'))
			return (false);
		token.type = TokenType::NUMBER;
		return (true);
	}
	return (false);
}

/*
bool Lexer::matchFile(Token &token)
{
	while (isPath(peek()))
		token.value += advance();
	if (token.value.empty() || peek() != '.')
		return (false);
	token.value += advance();
	while (isAlphaNum(peek()))
		token.value += advance();
	if (token.value.empty() || !(peek() == ';' || isWhitespace(peek())))
		return (false);
	token.type = TokenType::LOCATION_VALUE;
	return (true);
}
*/

Token Lexer::getNextToken()
{
	Token token;
	skipWhitespace();
	skipComment();
	token.line = _line;
	token.column = _column;
	char c = peek();
	(void)c;
	if (matchSpecialChar(token))
		return (token);
	if (matchPath(token))
		return (token);
	if (matchDirective(token))
		return (token);
	if (matchDigit(token))
		return (token);
	if (matchDomain(token))
		return (token);
	throw std::runtime_error("Unexpected character '" + std::string(1, peek()) + "' at line " + toString(_line) + " column " + toString(_column) + ". Last token: " + token.value);
}
