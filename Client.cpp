/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:26:31 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/12 01:25:45 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int _id, int _client_fd) 
    : id(_id), client_fd(_client_fd), nickname(), state(LOCKED), username(), realname(), op(false)  {}
    
Client::Client() : id(), nickname(), state(LOCKED), username(), realname(), op(false) {}

Client::~Client() {}

int Client::setNickname(std::string name) 
{
    std::cout << "state in nick: " << state <<std::endl;
    if (state == LOCKED)
        return (-8);
    if (state == SET || nickname.size() > 0)
        return (-7);
    nickname = name;
    upgradeState();
    return (-5);
}

const std::string &Client::getNickname()
{
    return (nickname);
}

const Client::State &Client::getState()
{
    return (state);
}

const std::string &Client::getUsername() 
{
    return (username);
}

int     Client::setUsername(std::string name)
{
    std::cout << "State in user: " << state<< std::endl;
    if (name.empty())
        return(-1);
    
    if (state == LOCKED)
        return (-2);

    if (state == SET || username.size() > 0)
        return (-3);
    
    username = name ;
    upgradeState();
    return (-4);
}

bool    Client::is_op()
{
    return (op);
}

bool    Client::set_op(std::string pass)
{
    if (pass == "42")
    {
        op = true;
        return(true);
    }
    return (false);
}

const std::string &Client::getRealname() 
{
    return (realname);
}

int     Client::setRealname(std::string name)
{
    if (state == LOCKED || state == UNITIALIZED)
        return (-1);
    
    if (state == SET)
        return (-1);

    realname = name;
    return (0);
}

int    Client::getFd()
{   
    return (client_fd);
}


void    Client::upgradeState()
{
    if (this->state == LOCKED)
        this->state = UNITIALIZED;
    else if (this->state == UNITIALIZED)
        this->state = INITIALIZED;
    else if (this->state == INITIALIZED)
        this->state = SET;
    std::cout << "state after pass: " << state << std::endl;
}