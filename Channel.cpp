/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:41:27 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/12 19:33:17 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(std::string ch_name, Client &c) : name (ch_name)
{
    this->clients.insert(std::pair<std::string, Client*>(c.getNickname(), &c));
    this->owner = c.getNickname();
    this->invite_only = false;
    is_private = false;
    is_secret = false;
    op_topic = false;
    no_msg = false;
    moderated = false;
    user_limit = 10000;
    password = "";
}

    
Channel &Channel::operator=(const Channel &c)
{
    this->name = c.name;
    return (*this);
}

int Channel::broadcast(std::string message, int sender)
{
    std::map<std::string, Client*>::iterator it;
    
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (sender != it->second->getFd())
            send(it->second->getFd(), message.c_str(), message.length(), 0);
    }
    return (0);
}

int Channel::connect(Client &c)
{
    if(this->invite_only)
    {
        std::vector<std::string>::iterator it = std::find(invited_users.begin(), invited_users.end(), c.getNickname());
        if(it == invited_users.end())
            return(-1);
    }
    clients.insert(std::pair<std::string, Client*>(c.getNickname(), &c));
    return(0);
}

int Channel::disconnect(Client &c)
{
    clients.erase(c.getNickname());
    if (clients.empty())
        return (1);
    return(0);
}

int Channel::disconnect(std::string nickname)
{
    clients.erase(nickname);
    if (clients.empty())
        return (1);
    return (0);
}

int Channel::client_count()
{
    return(clients.size());
}

int Channel::cmd_kick(std::string nickname)
{
        this->clients.erase(nickname);
        return(0);
}

int Channel::cmd_invite(std::string nickname)
{
    this->invited_users.push_back(nickname);
    return(0);
}

int Channel::cmd_topic(std::string top)
{
    this->topic = top;
    return (0);
}

bool    Channel::get_invite_only() const
{
    return (invite_only);
}

bool    Channel::get_is_private() const 
{
    return (is_private);
}

bool    Channel::get_op_topic() const 
{
    return(op_topic);
}

bool    Channel::get_no_msg() const 
{
    return (no_msg);
}

bool    Channel::get_moderated() const 
{
    return (moderated);
}

int    Channel::get_user_limit() const 
{
    return (user_limit);
}

std::string Channel::get_password() const 
{
    return (password);
}

std::vector<std::string> Channel::get_who_speak_on_moderated() const 
{
    return (who_speak_on_moderated);
}

bool   Channel::can_speak_on_channel(std::string nick)
{
    std::vector<std::string>::iterator it = std::find(who_speak_on_moderated.begin(), who_speak_on_moderated.end(), nick);
    if (it == who_speak_on_moderated.end() && !is_operator(nick))
        return (false);
    return (true);
}
std::vector<std::string>  Channel::get_invited_user() const
{
    return (invited_users);
}

std::vector<std::string> Channel::get_operators() const
{
    return (operators);
}


int Channel::add_operator(std::string nick)
{
    this->operators.push_back(nick);
    return (0);
}

int Channel::change_operator(std::string sign, std::string nick)
{
    std::cout << "change operator" << std::endl;
    if (sign == "-")
    {
        std::vector<std::string>::iterator p = std::find(operators.begin(), operators.end(), nick);
        if (p != operators.end())
            operators.erase(p);
    }
    else if (sign == "+")
        operators.push_back(nick);
    return(0);
}

int     Channel::change_to_private(std::string sign)
{
    std::cout << "change to private" << std::endl;
    if (sign == "-")
        is_private = false;
    else if (sign == "+")
        is_private = true;
    return(0);
}

int    Channel::change_to_secret(std::string sign) // change channel privacy
{
    std::cout << "change to secret" << std::endl;
    if (sign == "-")
        is_secret = false;
    else if (sign == "+")
        is_secret = true;
    return(0);
}

int Channel::change_optopic(std::string sign) // change topic privacy
{
    std::cout << "change optopic" << std::endl;
    if (sign == "-")
        op_topic = false;
    else if (sign == "+")
        op_topic = true;
    return(0);
}

int Channel::change_nomsg(std::string sign) // change message privacy
{
    std::cout << "change nomsg" << std::endl;
    if (sign == "-")
        no_msg = false;
    else if (sign == "+")
        no_msg = true;
    return(0);
}

int		Channel::change_moderated(std::string sign) // 
{
	std::cout<<"change moderated"<<std::endl;
	if (sign == "-") 
		moderated = false;
	else if (sign == "+")
		moderated = true;
	return (0);
}

int		Channel::change_userlimits(std::string sign, std::size_t limit)
{
	std::cout<<"change user limits"<<std::endl;
	if (sign == "-")
		user_limit = 10000;
	else if (sign == "+")
		user_limit = limit;
	return (0);
}

int		Channel::change_password(std::string sign, std::string key)
{
	std::cout<<"change password"<<std::endl;
	if (sign == "-")
		password = "";
	else if (sign == "+")
		password = key;
	return (0);
}

int		Channel::change_invite(std::string sign)
{
	std::cout<<"change invite"<<std::endl;
	if (sign == "-")
		invite_only = false;
	else if (sign == "+")
		invite_only = true;
	return (0);
}

int Channel::change_who_speaks_on_moderated(std::string sign, std::string user)
{
    std::cout << "change who speaks on moderated" << std::endl;
    if (sign == "-")
    {
        std::vector<std::string>::iterator p = std::find(who_speak_on_moderated.begin(), who_speak_on_moderated.end(), user);
        if (p != who_speak_on_moderated.end())
            {
                who_speak_on_moderated.erase(p);
                return (0);
            }
            return (-1); 
    }
    else if (sign == "+")
    {
        who_speak_on_moderated.push_back(user);
        return (0);
    }
}

bool Channel::limit_full()
{
    std::cout << "user limit: " << user_limit << " clients size: " << clients.size() << std::endl;
    if (user_limit > clients.size() || user_limit == 0)
        return (false);
    return (true);
}

bool Channel::is_invited(std::string nick)
{
    std::vector<std::string>::iterator p = std::find(invited_users.begin(), invited_users.end(), nick);
    if (p == invited_users.end())
        return (false);
    return (true);
}

int Channel::cmd_names(Client &sender) // send lsit of clients in channel
{
    if (get_is_secret())
    {
        if (!is_member(sender.getNickname())) // check if user is member of channel
        {
            return (-4); // user not in channel
        }
    }
    std::string listusers = ":IRC 353 " + sender.getNickname() + " = " + get_name() + " :"; 
    for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++) // loop through clients
    {
        if (is_operator(it->second->getNickname())) // check if client is operator
            listusers += "@";
        listusers += it->second->getNickname();
        std::map<std::string, Client*>:: iterator it2 = it;
        if ((++it2) != clients.end()) // check if next client is not the last
            listusers += " ";
    }
    listusers += "\r\n";
    send(sender.getFd(), listusers.c_str(), listusers.length(), 0); // send list of clients 
    std::string endmsg = ":IRC 366 " + sender.getNickname() + " " + get_name() + " end of /NAMES list. \r\n";
    return (0);
}