/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:41:27 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/16 16:56:19 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "client_irc.hpp"

Channel::Channel(){
    
}

Channel::~Channel(){
    
}

Channel::Channel(std::string ch_name, Client_irc *c) : name (ch_name)   
{
    this->owner = c->get_nick();
    this->invite_only = false;
    is_private = false;
    is_secret = false;
    op_topic = false;
    no_msg = false;
    moderated = false;
    user_limit = 256;
    key = "";
    this->clients.insert(std::make_pair(c->get_nick(), c));
}

    
Channel &Channel::operator=(const Channel &c)
{
    this->name = c.name;
    return (*this);
}

// int Channel::broadcast(std::string message, int sender) // send message to all clients
// {
//     std::map<std::string, Client_irc>::iterator it;
    
//     for (it = clients.begin(); it != clients.end(); it++)
//     {
//         if (sender != it->second.fd_client)
//             send(it->second.fd_client, message.c_str(), message.length(), 0);
//     }
//     return (0);
// }

// int Channel::connect(Client_irc &c) // add client to channel
// {
//     if(this->invite_only)
//     {
//         std::vector<std::string>::iterator it = std::find(invited_users.begin(), invited_users.end(), c.getNickname());
//         if(it == invited_users.end())
//             return;
//     }
//     clients[c.get_nick()] = c;
//     return(0);
// }

// int Channel::disconnect(Client_irc &c) // remove client from channel
// {
//     clients.erase(c.get_nick());
//     if (clients.empty())
//         return (1);
//     return(0);
// }

// int Channel::disconnect(std::string nickname) // remove client from channel
// {
//     clients.erase(nickname);
//     if (clients.empty())
//         return (1);
//     return (0);
// }

// // int Channel::client_count() // return number of clients in channel
// // {
// //     return(clients.size());
// // }

// int Channel::cmd_kick(std::string nickname) // kick client from channel

// {
//         this->clients.erase(nickname);
//         return(0);
// }

// int Channel::cmd_invite(std::string nickname)   // invite client to channel
// {
//     this->invited_users.push_back(nickname);
//     return(0);
// }

// int Channel::cmd_topic(std::string top) // change channel topic
// {
//     this->topic = top;
//     return (0);
// }

// bool    Channel::get_invite_only() const    // return invite only status
// {
//     return (invite_only);
// }

// bool    Channel::get_is_private() const     // return private status
// {
//     return (is_private);
// }


// bool    Channel::get_op_topic() const     // return topic privacy status
// {
//     return(op_topic);
// }

// bool    Channel::get_no_msg() const     // return message privacy status
// {
//     return (no_msg);
// }

// bool    Channel::get_moderated() const   // return moderated status
// {
//     return (moderated);
// }

// int    Channel::get_user_limit() const  // return user limit
// {
//     return (user_limit);
// }

// std::string Channel::get_password() const   // return channel password
// {
//     return (password);
// }

// std::vector<std::string> Channel::get_who_speak_on_moderated() const    // return who can speak on moderated channel
// {
//     return (who_speak_on_moderated);
// }

// bool   Channel::can_speak_on_channel(std::string nick)  // check if user can speak on moderated channel
// {
//     std::vector<std::string>::iterator it = std::find(who_speak_on_moderated.begin(), who_speak_on_moderated.end(), nick);
//     if (it == who_speak_on_moderated.end() && !is_operator(nick))
//         return (false);
//     return (true);
// }

// std::vector<std::string>  Channel::get_invited_user() const // return list of invited users
// {
//     return (invited_users);
// }

// std::vector<std::string> Channel::get_operators() const // return list of operators
// {
//     return (operators);
// }


// int Channel::add_operator(std::string nick) // add operator to channel
// {
//     this->operators.push_back(nick);
//     return (0);
// }

// int Channel::change_operator(std::string sign, std::string nick)    // change operator status
// {
//     std::cout << "change operator" << std::endl;
//     if (sign == "-")
//     {
//         std::vector<std::string>::iterator p = std::find(operators.begin(), operators.end(), nick);
//         if (p != operators.end())
//             operators.erase(p);
//     }
//     else if (sign == "+")
//         operators.push_back(nick);
//     return(0);
// }

// int     Channel::change_to_private(std::string sign) // change channel privacy
// {
//     std::cout << "change to private" << std::endl;
//     if (sign == "-")
//         is_private = false;
//     else if (sign == "+")
//         is_private = true;
//     return(0);
// }

// int    Channel::change_to_secret(std::string sign) // change channel privacy
// {
//     std::cout << "change to secret" << std::endl;
//     if (sign == "-")
//         is_secret = false;
//     else if (sign == "+")
//         is_secret = true;
//     return(0);
// }

// int Channel::change_optopic(std::string sign) // change topic privacy
// {
//     std::cout << "change optopic" << std::endl;
//     if (sign == "-")
//         op_topic = false;
//     else if (sign == "+")
//         op_topic = true;
//     return(0);
// }

// int Channel::change_nomsg(std::string sign) // change message privacy
// {
//     std::cout << "change nomsg" << std::endl;
//     if (sign == "-")
//         no_msg = false;
//     else if (sign == "+")
//         no_msg = true;
//     return(0);
// }

// int		Channel::change_moderated(std::string sign) // change moderated status
// {
// 	std::cout<<"change moderated"<<std::endl;
// 	if (sign == "-") 
// 		moderated = false;
// 	else if (sign == "+")
// 		moderated = true;
// 	return (0);
// }

// int		Channel::change_userlimits(std::string sign, std::size_t limit) // change user limit
// {
// 	std::cout<<"change user limits"<<std::endl;
// 	if (sign == "-")
// 		user_limit = 10000;
// 	else if (sign == "+")
// 		user_limit = limit;
// 	return (0);
// }

// int		Channel::change_password(std::string sign, std::string key) // change channel password
// {
// 	std::cout<<"change password"<<std::endl;
// 	if (sign == "-")
// 		password = "";
// 	else if (sign == "+")
// 		password = key;
// 	return (0);
// }

// int		Channel::change_invite(std::string sign) // change invite only status
// {
// 	std::cout<<"change invite"<<std::endl;
// 	if (sign == "-")
// 		invite_only = false;
// 	else if (sign == "+")
// 		invite_only = true;
// 	return (0);
// }

// int Channel::change_who_speaks_on_moderated(std::string sign, std::string user) // change who can speak on moderated channel
// {
//     std::cout << "change who speaks on moderated" << std::endl;
//     if (sign == "-")
//     {
//         std::vector<std::string>::iterator p = std::find(who_speak_on_moderated.begin(), who_speak_on_moderated.end(), user);
//         if (p != who_speak_on_moderated.end())
//             {
//                 who_speak_on_moderated.erase(p);
//                 return (0);
//             }
//             return (-1); 
//     }
//     else if (sign == "+")
//     {
//         who_speak_on_moderated.push_back(user);
//         return (0);
//     }
// }

// bool Channel::limit_full() // check if channel is full
// {
//     std::cout << "user limit: " << user_limit << " clients size: " << clients.size() << std::endl;
//     if (user_limit > clients.size() || user_limit == 0)
//         return (false);
//     return (true);
// }

// bool Channel::is_invited(std::string nick) // check if user is invited
// {
//     std::vector<std::string>::iterator p = std::find(invited_users.begin(), invited_users.end(), nick);
//     if (p == invited_users.end())
//         return (false);
//     return (true);
// }

// /********************************/

// // int Channel::cmd_names(Client &sender) // send lsit of clients in channel
// // {
// //     if (get_is_secret())
// //     {
// //         if (!is_member(sender.getNickname())) // check if user is member of channel
// //         {
// //             return (-4); // user not in channel
// //         }
// //     }
// //     std::string listusers = ":IRC 353 " + sender.getNickname() + " = " + get_name() + " :"; 
// //     for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++) // loop through clients
// //     {
// //         if (is_operator(it->second->getNickname())) // check if client is operator
// //             listusers += "@";
// //         listusers += it->second->getNickname();
// //         std::map<std::string, Client*>:: iterator it2 = it;
// //         if ((++it2) != clients.end()) // check if next client is not the last
// //             listusers += " ";
// //     }
// //     listusers += "\r\n";
// //     send(sender.getFd(), listusers.c_str(), listusers.length(), 0); // send list of clients 
// //     std::string endmsg = ":IRC 366 " + sender.getNickname() + " " + get_name() + " end of /NAMES list. \r\n";   // end of names list
// //     send(sender.getFd(), endmsg.c_str(), endmsg.length(),0); // send end of list
    
// //     return (0);
// // }
// /************************************************/


// std::string Channel::channel_modes() // return channel modes
// {
//     std::string modes;
//     if (operators.size())
//         modes += "o";
//     if (is_private)
//         modes += "p";
//     if (is_secret)
//         modes += "s";
//     if (invite_only)
//         modes += "i";
//     if (op_topic)
//         modes += "t";
//     if (no_msg)
//         modes += "n";
//     if (moderated)
//         modes += "m";
//     if (user_limit != 10000)
//         modes += "l";
//     if (who_speak_on_moderated.size())
//         modes += "v";
//     if (password != "")
//         modes += "k";
//     return (modes);
// }

// // int Channel::cmd_who(Client &sender) // send list of clients in channel
// // {
// //     for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
// //     {
// //         std::string listuser = ":IRC 352 " + sender.getNickname() +  " " + get_name() +  " " + it->second->getUsername() + " localhost IRC " + it->second->getNickname() + " H";
// //         if ( is_operator(it->second->getNickname()))
// //             listuser += "*";listuser += " @";
// //         listuser += " :0 " + it->second->getRealname() + "\r\n";
// //         send(sender.getFd(), listuser.c_str(), listuser.length(), 0); 
// //     }
// //     std::string msg = ":IRC 315 " + sender.getNickname() + " " + get_name() + " :End of /WHO list\r\n";
// //     send(sender.getFd(), msg.c_str(), msg.length(), 0);
// //     return (0);
// // }


// bool Channel::is_operator(std::string nick) // check if user is operator
// {
//     for (std::vector<std::string>::iterator p  = operators.begin(); p != operators.end(); p++)
//     {
//         if (nick == *p)
//             return (true);
//     } 
//     if (nick == owner)
//         return (true);
//     return (false);
// }

// std::string Channel::get_topic()
// {
//     return (topic);
// }

// std::string Channel::get_name() const
// {
//     return (name);
// }

// Client *Channel::get_channel_operator() const
// {
//     std::map<std::string, Client*>::const_iterator it = clients.find(owner);
//     return (it->second);
// }

// bool Channel::is_member(Client &c)  // check if user is member by client object
// {
//     return (is_member(c.getNickname()));
// }

// int Channel::can_invite(std::string nickname) // check by nickname if user can invite
// {
//     if (is_operator(nickname))
//         return (0);
//     if (invite_only)
//         return (-1);
//     std::map<std::string, Client*>::iterator it = clients.find(nickname);
//     if (it != clients.end())
//         return (0);
//     return (-1);
// }

// bool Channel::can_invite(Client &c) // check by client object if user can invite
// {
//     return (can_invite(c.getNickname()));
// }

//--
void    Channel::set_key(std::string    k){
    key = k;
}
//--
std::string Channel::get_key(){
    return (key);
}