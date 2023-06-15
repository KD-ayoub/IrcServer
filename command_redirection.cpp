/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redirection.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:31:06 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/15 16:55:25 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_irc.hpp"
#include "Channel.hpp"




// int Channel::join_command(std::vector<std::string> const &command, Client_irc &client) 
// {
//     if (channel_list.find(command[1]) == channel_list.end()) // if channel doesn't exist
//     {
//         Channel *new_channel = new Channel(command[1], client);
//         channel_list.insert(std::pair<std::string, Channel*>(command[1], new_channel));
//         std::cout << "Channel " << command[1] << " created" << std::endl;
//         std::cout << "Client " << client.get_nick() << " is now operator of " << command[1] << std::endl;
//         return(0);
//     }
//     else
//     {
//         channel_list[command[1]]->connect(client);
//     }
// }


// int Channel::kick_cmd(std::vector<std::string> const &command, Client_irc &client) 
// {
//     std::string client_channel;
//     if (channel_list.find(client_channel) == channel_list.end()) // if channel doesn't exist
//     {
//         std::cout << "can't KICK "<< client.get_nick() << " from this channel: " << std::endl;
//         return(0);
//     }
//     else
//     {
//         channel_list[client_channel].erase(client.get_nick());
//     }
// }

