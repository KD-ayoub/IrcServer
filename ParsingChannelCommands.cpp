/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingChannelCommands.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:16:08 by akadi             #+#    #+#             */
/*   Updated: 2023/06/11 16:58:00 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsingChannelCommands.hpp"

void    Parsing::setCommand(std::string cmd)
{
    this->_command = cmd;
}

void    Parsing::setFlag(std::string flg)
{
    this->_flag = flg;
}

std::string Parsing::getCommand()
{
    return this->_command;
}

std::string Parsing::getFlag()
{
    return this->_flag;
}

void    Parsing::ParseCmd(char *buffer, int recvalue)
{
    std::string cleanBuff;
    for(int i = 0; i < recvalue; i++)
        cleanBuff.push_back(buffer[i]);
    std::vector<std::string> split = SplitString(cleanBuff);
}