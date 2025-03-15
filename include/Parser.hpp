/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:45:23 by fmol              #+#    #+#             */
/*   Updated: 2025/03/12 15:02:09 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "ServerConfig.hpp"

class Parser
{
public:
	Parser(Lexer &lexer);
	~Parser();
	ConfigBlock parse();
private:
	Token &advance();
	void parseListenDirective(ConfigBlock &config_block);
	void parseServerNameDirective(ConfigBlock &config_block);
	void parseRootDirective(ConfigBlock &config_block);
	void parseIndexDirective(ConfigBlock &config_block);
	void parseErrorPageDirective(ConfigBlock &config_block);
	void parseMaxSizeDirective(ConfigBlock &config_block);
	void parseReturnDirective(ConfigBlock &config_block);
	ConfigBlock parseServerBlock();
	ConfigBlock parseLocationBlock();
	ConfigBlock parseAllowBlock();
	std::string getErrMsg(std::string const &expected, std::string const &found);

	Token _curr_token;
	Lexer &_lexer;
};

#endif // PARSER_HPP