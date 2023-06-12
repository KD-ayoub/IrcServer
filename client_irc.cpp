/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:55:05 by akouame           #+#    #+#             */
/*   Updated: 2023/06/12 18:49:43 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_irc.hpp"

Client_irc::Client_irc(){
    // std::cout << "Client_irc, default constructor called !" << std::endl;
	registred = false;
	_user.valid = false;
}
Client_irc::Client_irc(int fd_clt): fd_client(fd_clt){
	// std::cout << "Client_irc, parameter constructor called !" << std::endl;
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
void	Client_irc::set_registred(bool valid){
	registred = valid;
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
bool	Client_irc::get_registred(){
	return (registred);
}

//--
std::string	Client_irc::check_pass_cmd(char *buf, std::string pwd)
{
    std::string pass_cmd;

	
		  std::string msg = ":irc.1337.com 462 * :You may not reregister\r\n";
                    send_msg_to_client(msg.c_str());
    if (strlen(buf) < 6)
	{
		send_msg_to_client(":Password Incorrect");
        return ("");
	}
    int i = 4;
    while (buf[++i])
        pass_cmd += buf[i];
		// this line just for check buffer !
	std::cout << "pass_cmd = " << pass_cmd << "|" << std::endl;
		//-----
	if (pass_cmd.empty())
	{
		send_msg_to_client(":You may not reregister");
		return ("");
	}
    if (pass_cmd != pwd)
    {
		send_msg_to_client(":You may not reregister");
        return ("");
    }
    return (pass_cmd);
}
std::string	Client_irc::check_nick_cmd(char *buf)
{
    std::string nick_cmd;
    
    if (strlen(buf) < 6)
	{
		send_msg_to_client(":No nickname given");
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
		send_msg_to_client("USER :Not enough parameters");
		return(false);
	}
	if (user_splited[3][0] != ':')
	{
		send_msg_to_client("USER :The realname must start with  \':\'");
		return (false);
	}
	_user.username = user_splited[0];
	_user.hostname = user_splited[1];
	_user.servername = user_splited[2];
	for (size_t i = 3; i < user_splited.size(); i++)
		_user.realname += user_splited[i];
	_user.realname.erase(0, 1);
    return (true);
}
//--
bool    Client_irc::parse_registration(char *buf, std::string pwd)
{
    if (registred)
    {
		send_msg_to_client(":You may not reregister");
        return (true);
    }
    std::string tmp;
	for (int i = 0; i < 5; i++)
		tmp += buf[i];
	if (tmp == "PASS ")
	{
		_pass = check_pass_cmd(buf, pwd);
		if (_pass.empty())
			return (false);
		send_msg_to_client("--PASS added succesfully--");
		// std::cout << "--PASS added succesfully !--" << std::endl;
	}
	else if (tmp == "NICK ")
	{
		if (_pass.empty())
		{
			send_msg_to_client("You must add PASS before !");
			return (false);
		}
		_nick = check_nick_cmd(buf);
		if (_nick.empty())
			return (false);
		send_msg_to_client("--NICK added succesfully--");
		// std::cout << "--NICK added succesfully !--" << std::endl;
	}
	else if (tmp == "USER ")
	{
		if (_nick.empty() || _pass.empty())
		{
			send_msg_to_client("You must add PASS && NICK before !");
			// std::cout << "You must add PASS && NICK before !" << std::endl;
			return (false);	
		}
		if (!check_user_cmd(buf))
			return (false);
		_user.valid = true;
		send_msg_to_client("--USER added succesfully--");
		// std::cout << "--USER added succesfully !--" << std::endl;
	}
	else
		send_msg_to_client(ERR_UNKNOWNCOMMAND);// "<command> :Unknown command"
    if (!_pass.empty() && !_nick.empty() && !_user.valid == true)
		registred = true;
	if (!registred)
		return (false);
	return (true);
}
//--
void	Client_irc::send_msg_to_client(const char *msg)
{
	if (send(fd_client, msg, strlen(msg),0) == -1)
			std::perror("send");
}