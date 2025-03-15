/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:47:27 by fmol              #+#    #+#             */
/*   Updated: 2025/03/12 09:58:43 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(Lexer &lexer) : _lexer(lexer)
{
	_curr_token = _lexer.getNextToken();
}

Parser::~Parser()
{
}

Token &Parser::advance()
{
	_curr_token = _lexer.getNextToken();
	return (_curr_token);
}

std::string Parser::getErrMsg(std::string const &expected, std::string const &found)
{
	return ("Expected '" + expected + "' but found '" + found + "' at line " + toString(_curr_token.line) + " column " + toString(_curr_token.column));
}

void Parser::parseListenDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::LISTEN)
		throw std::runtime_error(getErrMsg("listen", _curr_token.value));
	advance();
	if (_curr_token.type == TokenType::NUMBER)
	{
		std::vector<std::string> directive_values;
		directive_values.push_back(_curr_token.value);
		config_block.directives["listen"] = directive_values;
		advance();
	}
	else if (_curr_token.type == TokenType::IP_VALUE)
	{
		std::vector<std::string> directive_values;
		directive_values.push_back(_curr_token.value);
		advance();
		if (_curr_token.type == TokenType::COLON)
		{
			advance();
			if (_curr_token.type == TokenType::NUMBER)
			{
				directive_values.push_back(_curr_token.value);
				config_block.directives["listen"] = directive_values;
				advance();
			}
			else
				throw std::runtime_error(getErrMsg("Number", _curr_token.value));
		}
		else
			throw std::runtime_error(getErrMsg(":", _curr_token.value));
	}
	else
		throw std::runtime_error(getErrMsg("Number or Ip Value", _curr_token.value));
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
}

void Parser::parseServerNameDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::SERVER_NAME)
		throw std::runtime_error(getErrMsg("server_name", _curr_token.value));
	advance();
	std::vector<std::string> directive_values;
	while (_curr_token.type != TokenType::SEMICOLON)
	{
		if (_curr_token.type != TokenType::DOMAIN_VALUE)
			throw std::runtime_error(getErrMsg("Domain Value", _curr_token.value));
		directive_values.push_back(_curr_token.value);
		advance();
	}
	config_block.directives["server_name"] = directive_values;
	advance();
}

void Parser::parseRootDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::ROOT)
		throw std::runtime_error(getErrMsg("root", _curr_token.value));
	advance();
	if (_curr_token.type != TokenType::PATH)
		throw std::runtime_error(getErrMsg("Path", _curr_token.value));
	std::vector<std::string> directive_values;
	directive_values.push_back(_curr_token.value);
	config_block.directives["root"] = directive_values;
	advance();
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
}

void Parser::parseReturnDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::RETURN)
		throw std::runtime_error(getErrMsg("return", _curr_token.value));
	advance();
	if (_curr_token.type != TokenType::NUMBER)
		throw std::runtime_error(getErrMsg("Number", _curr_token.value));
	std::vector<std::string> directive_values;
	directive_values.push_back(_curr_token.value);
	advance();
	if (_curr_token.type == TokenType::PATH)
	{
		directive_values.push_back(_curr_token.value);
		advance();
	}
	config_block.directives["return"] = directive_values;
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
}

void Parser::parseIndexDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::INDEX)
		throw std::runtime_error(getErrMsg("index", _curr_token.value));
	advance();
	std::vector<std::string> directive_values;
	switch (_curr_token.type)
	{
	case TokenType::AUTO:
		directive_values.push_back("auto");
		break;
	case TokenType::OFF:
		directive_values.push_back("off");
		break;
	case TokenType::PATH:
		directive_values.push_back(_curr_token.value);
		break;
	case TokenType::DOMAIN_VALUE: // because string with '.' might be recognized as domain
		directive_values.push_back(_curr_token.value);
		break;
	default:
		throw std::runtime_error(getErrMsg("auto, off or Path Value", _curr_token.value));
	}
	advance();
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
	config_block.directives["index"] = directive_values;
}

void Parser::parseErrorPageDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::ERROR_PAGE)
		throw std::runtime_error(getErrMsg("error_page", _curr_token.value));
	advance();
	std::vector<std::string> directive_values;
	if (_curr_token.type != TokenType::NUMBER)
		throw std::runtime_error(getErrMsg("Number", _curr_token.value));
	directive_values.push_back(_curr_token.value);
	advance();
	while (_curr_token.type != TokenType::PATH)
	{
		if (_curr_token.type != TokenType::NUMBER)
			throw std::runtime_error(getErrMsg("Number", _curr_token.value));
		directive_values.push_back(_curr_token.value);
		advance();
	}
	if (_curr_token.type != TokenType::PATH)
		throw std::runtime_error(getErrMsg("Path Value", _curr_token.value));
	directive_values.push_back(_curr_token.value);
	advance();
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
	config_block.directives["error_page"] = directive_values;
}

void Parser::parseMaxSizeDirective(ConfigBlock &config_block)
{
	if (_curr_token.type != TokenType::MAX_SIZE)
		throw std::runtime_error(getErrMsg("max_size", _curr_token.value));
	advance();
	if (_curr_token.type != TokenType::NUMBER)
		throw std::runtime_error(getErrMsg("Number", _curr_token.value));
	std::vector<std::string> directive_values;
	directive_values.push_back(_curr_token.value);
	config_block.directives["max_size"] = directive_values;
	advance();
	if (_curr_token.type != TokenType::SEMICOLON)
		throw std::runtime_error(getErrMsg(";", _curr_token.value));
	advance();
}

ConfigBlock Parser::parseAllowBlock()
{
	ConfigBlock config_block;
	if (_curr_token.type != TokenType::ALLOW)
		throw std::runtime_error(getErrMsg("allow", _curr_token.value));
	config_block.name = "allow";
	advance();
	std::vector<std::string> directive_values;
	while (_curr_token.type != TokenType::L_BRACE)
	{
		if (_curr_token.type != TokenType::REQUEST)
			throw std::runtime_error(getErrMsg("Request Value", _curr_token.value));
		directive_values.push_back(_curr_token.value);
		advance();
	}
	if (directive_values.size() == 0)
		throw std::runtime_error(getErrMsg("Request Value", _curr_token.value));
	config_block.directives["method"] = directive_values;
	if (_curr_token.type != TokenType::L_BRACE)
		throw std::runtime_error(getErrMsg("{", _curr_token.value));
	advance();
	if (_curr_token.type != TokenType::RETURN)
		throw std::runtime_error(getErrMsg("return", _curr_token.value));
	parseReturnDirective(config_block);
	if (_curr_token.type != TokenType::R_BRACE)
		throw std::runtime_error(getErrMsg("}", _curr_token.value));
	advance();
	return (config_block);
}

ConfigBlock Parser::parseLocationBlock()
{
	ConfigBlock config_block;
	if (_curr_token.type != TokenType::LOCATION)
		throw std::runtime_error(getErrMsg("location", _curr_token.value));
	config_block.name = "location";
	advance();
	if (_curr_token.type != TokenType::PATH)
		throw std::runtime_error(getErrMsg("Path Value", _curr_token.value));
	std::vector<std::string> directive_values;
	directive_values.push_back(_curr_token.value);
	config_block.directives["path"] = directive_values;
	advance();
	if (_curr_token.type != TokenType::L_BRACE)
		throw std::runtime_error(getErrMsg("{", _curr_token.value));
	advance();
	while (_curr_token.type != TokenType::R_BRACE)
	{
		if (_curr_token.type == TokenType::ROOT)
			parseRootDirective(config_block);
		else if (_curr_token.type == TokenType::INDEX)
			parseIndexDirective(config_block);
		else if (_curr_token.type == TokenType::ALLOW)
			config_block.children.push_back(parseAllowBlock());
		else if (_curr_token.type == TokenType::RETURN)
			parseReturnDirective(config_block);
		else
			throw std::runtime_error(getErrMsg("root, index, allow, or return", _curr_token.value));
	}
	advance();
	return (config_block);
}

ConfigBlock Parser::parseServerBlock()
{
	ConfigBlock config_block;
	if (_curr_token.type != TokenType::SERVER)
		throw std::runtime_error(getErrMsg("server", _curr_token.value));
	config_block.name = "server";
	advance();
	if (_curr_token.type != TokenType::L_BRACE)
		throw std::runtime_error(getErrMsg("{", _curr_token.value));
	advance();
	while (_curr_token.type != TokenType::R_BRACE)
	{
		if (_curr_token.type == TokenType::LISTEN)
			parseListenDirective(config_block);
		else if (_curr_token.type == TokenType::SERVER_NAME)
			parseServerNameDirective(config_block);
		else if (_curr_token.type == TokenType::ROOT)
			parseRootDirective(config_block);
		else if (_curr_token.type == TokenType::INDEX)
			parseIndexDirective(config_block);
		else if (_curr_token.type == TokenType::ERROR_PAGE)
			parseErrorPageDirective(config_block);
		else if (_curr_token.type == TokenType::MAX_SIZE)
			parseMaxSizeDirective(config_block);
		else if (_curr_token.type == TokenType::LOCATION)
			config_block.children.push_back(parseLocationBlock());
		else
			throw std::runtime_error(getErrMsg("listen, server_name, root, index, error_page, max_size, or location", _curr_token.value));
	}
	advance();
	return (config_block);
} 

ConfigBlock Parser::parse()
{
	ConfigBlock main_block;
	main_block.name = "main";
	while (_curr_token.type != TokenType::END)
	{
		if (_curr_token.type == TokenType::SERVER)
			main_block.children.push_back(parseServerBlock());
		else
			throw std::runtime_error(getErrMsg("server", _curr_token.value));
	}
	return (main_block);
}