/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:47:09 by akouame           #+#    #+#             */
/*   Updated: 2023/06/14 21:30:32 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_HPP
#define CLIENT_IRC_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include "ft_irc.hpp"

typedef struct User_parameters
{
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;
	bool	valid;
} User_parameters;
//--
typedef	struct	Msg_error
{
	std::string	ERR_NORECIPIENT;//
	std::string	ERR_NOTEXTTOSEND;//
	std::string	ERR_UNKNOWNCOMMAND;//
	std::string	ERR_NONICKNAMEGIVEN;//
	std::string	ERR_NICKNAMEINUSE;//
	std::string	ERR_NICKCOLLISION;//
	std::string	ERR_NOTREGISTERED;//
	std::string	ERR_NEEDMOREPARAMS;//
	std::string	ERR_ALREADYREGISTRED;//
	std::string	ERR_PASSWDMISMATCH;//

} Msg_error;
//--
class   Client_irc
{
	std::string	_pass;
	std::string	_nick;
	bool	_operator;
	User_parameters	_user;
	bool	registered;
	std::vector<std::string> _commands;
	public:
	std::string _stringtoappend;
	int size_cmds;
		std::string				buffer;
		int	fd_client;
		std::string	cmd;
		std::string	msg;
		Msg_error	error_msg;
		//--
		Client_irc();
		Client_irc(int fd_clt);
		~Client_irc();
		//--
		void	set_pass(std::string pwd);
		void	set_nick(std::string nck);
		void	set_user(User_parameters usr);
		void	set_registered(bool valid);
		void	set_stringtoappend(std::string);
		void	set_commands(const std::vector<std::string> &);
		void	set_msg_error();
		//--
		std::string	get_pass();
		std::string	get_nick();
		std::string get_stringtoappend();
		User_parameters	get_user();
		bool	get_registered();
		int	get_size_cmds();
		std::vector<std::string>	&get_commands();
		bool	is_op(); 
		//--
		std::string	check_pass_cmd(char *buf, std::string pwd);
		std::string	check_nick_cmd(char *buf);
		bool	check_user_cmd(char *buf);
		//--
		bool	parse_registration(char *buf, std::string pwd);
		//--
		void	send_msg_to_client();
		//--
		void	setup_user();
};
std::vector<std::string> split_string(const std::string &str, char delimiter);


#endif 