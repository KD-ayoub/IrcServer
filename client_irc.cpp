/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:55:05 by akouame           #+#    #+#             */
/*   Updated: 2023/06/12 14:29:55 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_irc.hpp"

Client_irc::Client_irc(){
    // std::cout << "Client_irc, default constructor called !" << std::endl;
	registred = false;
	_user.valid = false;
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
void	Client_irc::set_user(User_parameters	usr){
	_user.username = usr.username;
	_user.hostname = usr.hostname;
	_user.servername = usr.servername;
	_user.realname = usr.realname;
	_user.valid = usr.valid;
}
//--
std::string	Client_irc::get_pass(){
	return (_pass);
}
std::string	Client_irc::get_nick(){
	return (_nick);
}
User_parameters	Client_irc::get_user(){
	return (_user);
}
//--
std::string	Client_irc::check_pass_cmd(char *buf, std::string pwd)
{
    std::string pass_cmd;
 
    if (strlen(buf) < 6)
        return ("");
    int i = 4;
    while (buf[++i])
        pass_cmd += buf[i];
    if (pass_cmd != pwd)
    {
        std::cerr << "Incorrect password !" << std::endl;
        return ("");
    }
    return (pass_cmd);
}
std::string	Client_irc::check_nick_cmd(char *buf)
{
    std::string nick_cmd;
    
    if (strlen(buf) < 6)
	{
		std::cerr << ":No nickname given" <<std::endl;
        return ("");
	}
    int i = 4;
    while (buf[++i])
        nick_cmd[1] += buf[i];
    return (nick_cmd);
}
bool	Client_irc::check_user_cmd(char *buf)
{    
    std::string user_cmd;
	std::vector<std::string>	user_splited;
	
    int i = 4;
    while (buf[++i])
        user_cmd += buf[i];
	user_splited = split_string(user_cmd, ' ');
	if (user_splited.size() < 4)
	{
		std::cerr << "USER" << " :Not enough parameters" << std::endl;
		return(false);
	}
	if (user_splited[3][0] != ':')
	{
		std::cerr << "USER" << " :The realname must start with  \':\'" << std::endl;
		return (false);
	}
	_user.username = user_splited[0];
	_user.hostname = user_splited[1];
	_user.servername = user_splited[2];
	for (size_t i = 3; i < user_splited.size(); i++)
		_user.realname += user_splited[i];
    return (true);
}
//--
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
		_pass = check_pass_cmd(buf, pwd);
		if (_pass.empty())
		{
			std::cerr << "Empty password !" << std::endl;
			return (false);
		}
		std::cout << "--PASS added succesfully !--" << std::endl;
	}
	else if (tmp == "NICK ")
	{
		if (_pass.empty())
		{
			std::cout << "You must add PASS before !" << std::endl;
			return (false);
		}
		_nick = check_nick_cmd(buf);
		if (_nick.empty())
			return (false);
		std::cout << "--NICK added succesfully !--" << std::endl;
	}
	else if (tmp == "USER ")
	{
		if (_nick.empty() || _pass.empty())
		{
			std::cout << "You must add PASS && NICK before !" << std::endl;
			return (false);	
		}
		if (!check_user_cmd(buf))
			return (false);
		_user.valid = true;
		std::cout << "--USER added succesfully !--" << std::endl;
	}
    if (!_pass.empty() && !_nick.empty() && !_user.valid == true)
		registred = true;
	if (!registred)
		return (false);
	return (true);
}
