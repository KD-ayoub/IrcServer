/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:55:05 by akouame           #+#    #+#             */
/*   Updated: 2023/06/10 19:08:45 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_irc.hpp"

Client_irc::Client_irc(){
    // std::cout << "Client_irc, default constructor called !" << std::endl;

}

Client_irc::~Client_irc(){
    // std::cout << "Client_irc, default destructor called !" << std::endl;
    
}
//--
void	Client_irc::set_pass(std::string	pwd){
	_pass = pwd;
}
void	Client_irc::set_nick(std::string	nck){
	_nick = nck;
}
void	Client_irc::set_user(std::string	usr){
	_user = usr;
}
//--
std::string	Client_irc::get_pass(){
	return (_pass);
}
std::string	Client_irc::get_nick(){
	return (_nick);
}
std::string	Client_irc::get_user(){
	return (_user);
}
//--
std::string	Client_irc::check_pass_cmd(char *buf, std::string pwd)
{
    std::string pass_cmd;
 
    if (strlen(buf) < 6)
        return (NULL);
    int i = 4;
    while (buf[++i])
        pass_cmd += buf[i];
    if (pass_cmd != pwd)
    {
        std::cerr << "Incorrect password !" << std::endl;
        return (NULL);
    }
    return (pass_cmd);
}
std::string	Client_irc::check_nick_cmd(char *buf)
{
    std::string nick_cmd;
    
    if (strlen(buf) < 6)
        return (NULL);
    int i = 4;
    while (buf[++i])
        nick_cmd[1] += buf[i];
    return (nick_cmd);
}
std::string	Client_irc::check_user_cmd(char *buf)
{    
    std::string user_cmd;

    int i = 4;
    while (buf[++i])
        user_cmd += buf[i];
	
    return (user_cmd);
}

bool    Client_irc::parse_registration(char *buf, std::string pwd)
{
    if (registred)
    {
        std::cout << ":You may not reregister" << std::endl;
        return (true);
    }
    std::string tmp;
	for (int i = 0; i < 5; i++)
		tmp += buf[i];
	if (tmp == "PASS ")
	{
		if (check_pass_cmd(buf, pwd).empty())
			return (false);
		set_pass(check_pass_cmd(buf, pwd));
		std::cout << "--PASS added succesfully !--" << std::endl;
	}
	else if (tmp == "NICK ")
	{
		if (_pass.empty())
		{
			std::cout << "You must have PASS before !" << std::endl;
			return (false);
		}
		if (check_nick_cmd(buf).empty())
			return (false);
		set_nick(check_nick_cmd(buf));
		std::cout << "--NICK added succesfully !--" << std::endl;
	}
	else if (tmp == "USER ")
	{
		if (_pass.empty() || _pass.empty())
		{
			std::cout << "You must have PASS && NICK before !" << std::endl;
			return (false);	
		}
		if (check_user_cmd(buf).empty())
			return (false);
		set_user(check_user_cmd(buf));
		std::cout << "--USER added succesfully !--" << std::endl;
	}
    if (!_pass.empty() && !_nick.empty() && !_user.empty())
		registred = true;
	if (!registred)
		return (false);
	return (true);
}