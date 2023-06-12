/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 17:53:34 by akadi             #+#    #+#             */
/*   Updated: 2023/06/10 15:07:43 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int ac, char **av)
{
    checkArguments(ac, av);
    IrcServer irc(av[1], av[2]);
    int sockFd = irc.SetupServer();
    irc.AccetConnection(sockFd);
    
}