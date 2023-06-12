/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:47:09 by akouame           #+#    #+#             */
/*   Updated: 2023/06/10 21:50:22 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC_HPP
#define CLIENT_IRC_HPP

#include <string>

#include "ft_irc.hpp"

class   Client_irc
{
	std::string	_pass;
	std::string	_nick;
	std::string	_user;
	bool	registred;
	
	public:
		Client_irc();
		~Client_irc();
		//--
		void	set_pass(std::string pwd);
		void	set_nick(std::string nck);
		void	set_user(std::string usr);
		//--
		std::string	get_pass();
		std::string	get_nick();
		std::string	get_user();
		//--
		std::string	check_pass_cmd(char *buf, std::string pwd);
		std::string	check_nick_cmd(char *buf);
		std::string	check_user_cmd(char *buf);
		
		bool	parse_registration(char *buf, std::string pwd);

};

#endif 