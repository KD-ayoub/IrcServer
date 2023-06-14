/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:28:15 by akouame           #+#    #+#             */
/*   Updated: 2023/06/14 21:29:31 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "client_irc.hpp"

std::vector<std::string> split_string(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream string_stream(str);
    std::string token;

    while (std::getline(string_stream, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    return (tokens);
}

void    display_vct_str(std::vector<std::string> vct)
{
    for (size_t i = 0; i < vct.size(); i++)
        std::cout << vct[i] << std::endl;
}