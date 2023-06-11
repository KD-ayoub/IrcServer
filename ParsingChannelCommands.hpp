/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingChannelCommands.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:10:13 by akadi             #+#    #+#             */
/*   Updated: 2023/06/11 17:06:34 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCHANNELCOMMANDS_HPP
#define PARSINGCHANNELCOMMANDS_HPP

#include <iostream>
#include <vector>

class Parsing
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
        void    ParseCmd(char *, int);
};

std::vector<std::string> SplitString(std::string);
int countCmd(std::string str);

#endif
