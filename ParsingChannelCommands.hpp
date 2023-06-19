/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingChannelCommands.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:10:13 by akadi             #+#    #+#             */
/*   Updated: 2023/06/13 13:02:50 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCHANNELCOMMANDS_HPP
#define PARSINGCHANNELCOMMANDS_HPP

#include <iostream>
#include <vector>
#include <map>

class ParsingChannelCommands
{
    private :
        std::string _command;
        std::string _flag;
        std::vector<std::string> _args;
    public :
        void    setCommand(std::string);
        void    setFlag(std::string);
        
        std::string getCommand();
        std::string getFlag();
        void    ParseCmd(const char *, int);
};

void            SplitString(std::string, std::vector<std::string> &, int);
std::string     TrimString(std::string);
std::string Rtrim(const std::string &str);
std::string Ltrim(const std::string &str);
int             countCmd(std::string str);

#endif
