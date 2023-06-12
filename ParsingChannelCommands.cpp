/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingChannelCommands.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:16:08 by akadi             #+#    #+#             */
/*   Updated: 2023/06/11 19:35:31 by akadi            ###   ########.fr       */
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
    std::vector<std::string> split;
    for(int i = 0; i < recvalue; i++)
        cleanBuff.push_back(buffer[i]);
    // std::cout << cleanBuff;
    SplitString(cleanBuff, split);
    for(size_t i = 0; i < split.size(); i++)
        std::cout << split[i] << std::endl;
    split.clear();
}