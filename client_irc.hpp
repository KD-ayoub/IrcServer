/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:47:09 by akouame           #+#    #+#             */
/*   Updated: 2023/06/13 14:33:11 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_HPP
#define CLIENT_IRC_HPP

#include <string>
#include <vector>
#include <sstream>

// #define ERR_NORECIPIENT (cmd)	(":No recipient given "+cmd+"\r\n")
// #define ERR_NOTEXTTOSEND	":No text to send"+"\r\n" //
#include <iostream>
#include <string>

// #define ERR_UNKNOWNCOMMAND (std::string cmd)	cmd + " :Unknown command\r\n"
// #define ERR_NONICKNAMEGIVEN	":No nickname given"+"\r\n" //
// #define	ERR_NICKNAMEINUSE (client) client.get_nick()" :Nickname is already in use"+"\r\n"
// #define	ERR_NICKCOLLISION (client)	client.get_nick()+" :Nickname collision KILL"+"\r\n"
// #define	ERR_NOTREGISTERED	":You have not registered"+"\r\n" //
// #define	ERR_NEEDMOREPARAMS (std::string cmd)	cmd+" :Not enough parameters"+"\r\n"
// #define	ERR_ALREADYregistered	 ":You may not reregister"+"\r\n"
// #define	ERR_PASSWDMISMATCH	":Password incorrect"+"\r\n"

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
	User_parameters	_user;
	bool	registered;

	public:
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
		void	set_msg_error();
		//--
		std::string	get_pass();
		std::string	get_nick();
		User_parameters	get_user();
		bool	get_registered();
		//--
		std::string	check_pass_cmd(char *buf, std::string pwd);
		std::string	check_nick_cmd(char *buf);
		bool	check_user_cmd(char *buf);
		//--
		bool	parse_registration(char *buf, std::string pwd);
		//--
		void	send_msg_to_client();
};
std::vector<std::string> split_string(const std::string &str, char delimiter);


#endif 