/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:47:09 by akouame           #+#    #+#             */
/*   Updated: 2023/06/12 18:58:29 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_HPP
#define CLIENT_IRC_HPP

#include <string>
#include <vector>
#include <sstream>

#define ERR_NORECIPIENT	":No recipient given (<command>)"
#define ERR_NOTEXTTOSEND	":No text to send"
#define ERR_UNKNOWNCOMMAND	"<command> :Unknown command"
#define ERR_NONICKNAMEGIVEN	":No nickname given"
#define	ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"
#define	ERR_NICKCOLLISION	"<nick> :Nickname collision KILL"
#define	ERR_NOTREGISTERED	":You have not registered"
#define	ERR_NEEDMOREPARAMS	"<command> :Not enough parameters"
#define	ERR_ALREADYREGISTRED	 ":You may not reregister"
#define	ERR_PASSWDMISMATCH	":Password incorrect"
#define	

#include "ft_irc.hpp"

typedef struct User_parameters
{
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;
	bool	valid;
} User_parameters;

class   Client_irc
{
	std::string	_pass;
	std::string	_nick;
	User_parameters	_user;
	bool	registred;
	
	public:
		int	fd_client;
		char	*msg;
		//--
		Client_irc();
		Client_irc(int fd_clt);
		~Client_irc();
		//--
		void	set_pass(std::string pwd);
		void	set_nick(std::string nck);
		void	set_user(User_parameters usr);
		void	set_registred(bool valid);
		//--
		std::string	get_pass();
		std::string	get_nick();
		User_parameters	get_user();
		bool	get_registred();
		//--
		std::string	check_pass_cmd(char *buf, std::string pwd);
		std::string	check_nick_cmd(char *buf);
		bool	check_user_cmd(char *buf);
		//--
		bool	parse_registration(char *buf, std::string pwd);
		//--
		void	send_msg_to_client(const char *msg);
};
std::vector<std::string> split_string(const std::string &str, char delimiter);


#endif 