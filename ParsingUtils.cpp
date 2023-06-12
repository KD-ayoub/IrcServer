/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 16:59:19 by akadi             #+#    #+#             */
/*   Updated: 2023/06/11 20:12:56 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsingChannelCommands.hpp"

int countCmd(std::string str)
{
    int count = 0;
    for(size_t i = 0; i < str.size(); i++)
        if (str[i] == '\n')
            count++;
    return count;
}

void SplitString(std::string str, std::vector<std::string> &split)
{
    int cmd = countCmd(str);
    int i = 0;
    size_t pos = 0;
    while(i < cmd)
    {
        pos = str.find('\n');
        if (pos == std::string::npos)
            std::cout << "here\n";
        split.push_back(str.substr(0, pos));
        str.erase(0, pos + 1);
        i++;
    }
}