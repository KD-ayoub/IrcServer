/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:47:09 by akouame           #+#    #+#             */
/*   Updated: 2023/06/12 14:05:19 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_HPP
#define CLIENT_IRC_HPP

#include <string>
#include <vector>
#include <sstream>

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
	
	public:
		bool	registred;
		//--
		Client_irc();
		~Client_irc();
		//--
		void	set_pass(std::string pwd);
		void	set_nick(std::string nck);
		void	set_user(User_parameters usr);
		//--
		std::string	get_pass();
		std::string	get_nick();
		User_parameters	get_user();
		bool	get_registred;
		//--
		std::string	check_pass_cmd(char *buf, std::string pwd);
		std::string	check_nick_cmd(char *buf);
		bool	check_user_cmd(char *buf);
		//--
		bool	parse_registration(char *buf, std::string pwd);

};
std::vector<std::string> split_string(const std::string &str, char delimiter);


#endif 