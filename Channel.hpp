/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 23:20:42 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/12 01:39:54 by yel-qabl         ###   ########.fr       */
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
        int     invite(std::string nickname); // invite client to channel
        int     cmd_topic(std::string topic); // change channel topic
        int     cmd_names(Client &sender); // send list of clients in channel
        int     cmd_who(Client &sender); // send list of clients in channel 
        int     add_operator(std::string nick); // add operator to channel
        int     change_operator(std::string sign, std::string nick); // change operator status
        int     client_count(); // return number of clients in channel 
        int     list_cmd(Client &sender); // send list of command available in channel

        // setters

        int     change_to_private(std::string sign); // change channel privacy
        int     change_to_secret(std::string sign); 
        int     change_optopic(std::string sign); // change topic privacy
        int     change_nomsg(std::string sign);
        int     change_moderated(std::string sign);
        int     change_userlimits(std::string sign, std::size_t limit);
        int     change_password(std::string sign, std::string key);
        int     chnage_invite(std::string sign);
        int     change_who_speaks_on_moderated(std::string sign, std::string user);

        //getters

        std::string                  get_topic();
        std::string                  get_name() const;
        Client                      *get_operator() const;
        bool                         is_invited(std::string nick);
        bool                         is_operator(std::string nickname);
        bool                         is_member(Client &c);
        bool                         is_member(std::string nickname);
        bool                         can_invite(Client &c);
        int                          can_invite(std::string nickname);
        bool                         get_invite_only() const;
        bool                         get_is_private() const;
        bool                         get_is_secret() const;
        bool                         get_op_topic() const;
        bool                         get_no_msg() const;
        bool                         get_moderated() const;
        int                          get_user_limit() const;
        bool                         can_speak_on_channel(std::string nick);
        bool                         limit_full();
        std::string                  get_password() const;
        std::vector<std::string>     get_who_speak_on_moderated() const;
        std::vector<std::string>     get_invited_user() const;
        std::vector<std::string>     get_operators() const;
        std::string                  channel_modes();
        
    private:

        std::string                      name;
        std::string                      owner;
        std::vector<std::string>         operators;
        std::string                      topic;
        std::map<std::string, Client*>   clients;
        bool                             invite_only;
        std::vector<std::string>         invited_users;
        bool                             is_private;
        bool                             is_secret;
        bool                             op_topic;
        bool                             no_msg;
        bool                             moderated;
        std::size_t                      user_limit;
        std::string                      password;
        std::vector<std::string>         who_speak_on_moderated;
              
};

#endif 