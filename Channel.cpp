/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:41:27 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/12 01:54:43 by yel-qabl         ###   ########.fr       */
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
    }
}