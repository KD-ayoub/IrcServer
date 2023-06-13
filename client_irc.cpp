/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_irc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:55:05 by akouame           #+#    #+#             */
/*   Updated: 2023/06/13 15:34:37 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_irc.hpp"

void	Client_irc::set_msg_error()
{
	error_msg.ERR_NORECIPIENT = ":ircserv 411 * :No recipient given "+cmd+"\r\n";
	error_msg.ERR_UNKNOWNCOMMAND = ":ircserv 421 * :"+cmd+" Unknown command\r\n";
	error_msg.ERR_NICKNAMEINUSE = ":ircserv 433 * :"+_nick+" Nickname is already in use\r\n";
	error_msg.ERR_NICKCOLLISION = ":ircserv ERROR * :"+_nick+" Nickname collision KILL\r\n";
	error_msg.ERR_NEEDMOREPARAMS = ":ircserv 461 * :"+cmd+" Not enough parameters\r\n";
	error_msg.ERR_NOTEXTTOSEND = ":ircserv 412 * :No text to send\r\n";
	error_msg.ERR_NONICKNAMEGIVEN = ":ircserv 431 * :No nickname given\r\n";
	error_msg.ERR_NOTREGISTERED = ":ircserv 451 * :You have not registered\r\n";
	error_msg.ERR_ALREADYREGISTRED = ":ircserv 462 * :You may not reregister\r\n";
	error_msg.ERR_PASSWDMISMATCH = ":ircserv 464 * :Password incorrect\r\n";
}
			// msg = ":server ERROR * :You must add PASS before !\r\n";

Client_irc::Client_irc(){
    // std::cout << "Client_irc, default constructor called !" << std::endl;
	registered = false;
	_user.valid = false;
	set_msg_error();
	
}
Client_irc::Client_irc(int fd_clt): fd_client(fd_clt){
	// std::cout << "Client_irc, parameter constructor called !" << std::endl;
	registered = false;
	_user.valid = false;
	set_msg_error();
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
void	Client_irc::set_registered(bool valid){
	registered = valid;
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
bool	Client_irc::get_registered(){
	return (registered);
}

//--
std::string	Client_irc::check_pass_cmd(char *buf, std::string pwd)
{
    std::string pass_cmd;

    if (strlen(buf) < 6)
	{
		msg = error_msg.ERR_PASSWDMISMATCH;
		send_msg_to_client();
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
		msg = error_msg.ERR_PASSWDMISMATCH;
		send_msg_to_client();
		return ("");
	}
    if (pass_cmd != pwd)
    {
		msg = error_msg.ERR_PASSWDMISMATCH;
        send_msg_to_client();
		// send_msg_to_client(error_msg.ERR_PASSWDMISMATCH.c_str());
        return ("");
    }
    return (pass_cmd);
}
std::string	Client_irc::check_nick_cmd(char *buf)
{
    std::string nick_cmd;
    
    if (strlen(buf) < 6)
	{
		msg = ":No nickname given";
		send_msg_to_client();
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
		msg = "USER :Not enough parameters\r\n";
		send_msg_to_client();
		return(false);
	}
	if (user_splited[3][0] != ':')
	{
		msg = "USER :The realname must start with  \':\'";
		send_msg_to_client();
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
    if (registered)
    {
		msg = error_msg.ERR_ALREADYREGISTRED;
		send_msg_to_client();
        return (true);
    }
	cmd.clear();
	for (int i = 0; i < 5; i++)
		cmd += buf[i];
	if (cmd == "PASS ")
	{
		_pass = check_pass_cmd(buf, pwd);
		if (_pass.empty())
			return (false);
		msg = "--PASS added succesfully--";
		send_msg_to_client(); // check if i will send it like this or not !
	}
	else if (cmd == "NICK ")
	{
		if (_pass.empty())
		{
			msg = ":server ERROR * :You must add PASS before !\r\n";
			send_msg_to_client();
			return (false);
		}
		_nick = check_nick_cmd(buf);
		if (_nick.empty())
			return (false);
		msg = "--NICK added succesfully--";
		send_msg_to_client();
		// std::cout << "--NICK added succesfully !--" << std::endl;
	}
	else if (cmd == "USER ")
	{
		if (_nick.empty() || _pass.empty())
		{
			msg = "You must add PASS && NICK before !";
			send_msg_to_client();
			// std::cout << "You must add PASS && NICK before !" << std::endl;
			return (false);	
		}
		if (!check_user_cmd(buf))
			return (false);
		_user.valid = true;
		msg = "--USER added succesfully--";
		send_msg_to_client();
		// std::cout << "--USER added succesfully !--" << std::endl;
	}
	// else
		// send_msg_to_client(ERR_UNKNOWNCOMMAND(cmd));// "<command> :Unknown command"
    if (!_pass.empty() && !_nick.empty() && !_user.valid == true)
		registered = true;
	if (!registered)
		return (false);
	return (true);
}
//--
void	Client_irc::send_msg_to_client()
{
	std::cout << "msg = |" << msg << "|" << std::endl;
	// std::cout << "this is fd ==  " << fd_client << std::endl;
	if (send(fd_client, msg.c_str(), msg.length(),0) == -1)
			std::perror("send");
}

//   std::string msg = ":irc.1337.com 462 * :You may not reregister\r\n";
//                     send_msg_to_client(msg.c_str());