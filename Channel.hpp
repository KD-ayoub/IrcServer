/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 23:20:42 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/14 17:17:44 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <sys/socket.h>
#include "Client.hpp"

class Channel {
    public:
        Channel(std::string name, Client &c);
        ~Channel();
        Channel &operator=(const Channel &c);

        
        int     broadcast(std::string message, int sender); // send message to all clients
        int     connect(Client &c); // add client to channel
        int     disconnect(Client &c); // remove client from channel
        int     disconnect(std::string nickname); // remove client from channel
        int     cmd_kick(std::string nickname); // kick client from channel
        int     cmd_invite(std::string nickname); // invite client to channel
        int     cmd_topic(std::string topic); // change channel topic
        int     cmd_names(Client &sender); // send list of clients in channel
        int     cmd_who(Client &sender); // send list of clients in channel 
        int     add_operator(std::string nick); // add operator to channel
        int     change_operator(std::string sign, std::string nick); // change operator status
        int     client_count(); // return number of clients in channel 
        int     list_cmd(Client &sender); // send list of command available in channel

        // setters

        int     change_to_private(std::string sign); // change channel privacy
        int     change_to_secret(std::string sign);  // change channel privacy
        int     change_optopic(std::string sign); // change topic privacy
        int     change_nomsg(std::string sign); // change message privacy
        int     change_moderated(std::string sign);     // change moderated status
        int     change_userlimits(std::string sign, std::size_t limit); // change user limit
        int     change_password(std::string sign, std::string key); // change channel password
        int     change_invite(std::string sign); // change invite only status
        int     change_who_speaks_on_moderated(std::string sign, std::string user); // change who can speak on moderated channel

        //getters

        std::string                  get_topic(); // return channel topic
        std::string                  get_name() const; // return channel name
        Client                      *get_channel_operator() const; // return channel operator
        bool                         is_invited(std::string nick); // check if user is invited
        bool                         is_operator(std::string nickname); // check if user is operator
        bool                         is_member(Client &c); // check if user is member by client object 
        bool                         is_member(std::string nickname); // check if user is member by nickname
        bool                         can_invite(Client &c); // check by client object if user can invite
        int                          can_invite(std::string nickname); // check by nickname if user can invite
        bool                         get_invite_only() const; // check if channel is invite only
        bool                         get_is_private() const; // check if channel is private
        bool                         get_is_secret() const; // check if channel is secret
        bool                         get_op_topic() const; // check if only operator can change topic
        bool                         get_no_msg() const; // check if only operator can send message
        bool                         get_moderated() const; // check if channel is moderated
        int                          get_user_limit() const; // return user limit
        bool                         can_speak_on_channel(std::string nick); // check if user can speak on moderated channel
        bool                         limit_full(); // check if channel is full
        std::string                  get_password() const; // return channel password
        std::vector<std::string>     get_who_speak_on_moderated() const; // return list of users who can speak on moderated channel
        std::vector<std::string>     get_invited_user() const; // return list of invited users
        std::vector<std::string>     get_operators() const; //  return list of operators
        std::string                  channel_modes();  // return channel modes 
        
    private:

        std::string                      name; // channel name
        std::string                      owner; // channel creator
        std::vector<std::string>         operators; // list of operators
        std::string                      topic; 
        std::map<std::string, Client*>   clients; // list of clients in channel  
        bool                             invite_only; // if true, only invited users can join
        std::vector<std::string>         invited_users; // list of users invited to channel
        bool                             is_private; // if true, only invited users can join
        bool                             is_secret; // if true, channel is not listed in /list
        bool                             op_topic; // if true, only operator can change topic
        bool                             no_msg; // if true, only operator can send message
        bool                             moderated; // if true, only users in who_speak_on_moderated can speak
        std::size_t                      user_limit; // max number of users in channel
        std::string                      password; // password to join channel
        std::vector<std::string>         who_speak_on_moderated; // list of users who can speak on moderated channel
              
};

#endif 