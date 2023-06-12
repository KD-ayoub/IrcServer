/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 23:37:51 by yel-qabl          #+#    #+#             */
/*   Updated: 2023/06/12 01:23:08 by yel-qabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

class Client {
    public :
        enum State 
        {
            LOCKED,
            UNITIALIZED,
            INITIALIZED,
            SET
        };
        
    public :
        Client(int id, int client_fd);
        Client();
        ~Client();

        Client &operator=(const Client &c);
        
        int         parse(std::string cmd);
        
        // getters
        
        const std::string          &getNickname();
        const Client::State        &getState();
        const std::string          &getUsername();
        const std::string          &getRealname();
        int                         getFd();
        bool                        is_op();
        

        // setters

        int                         setRealname(std::string name);
        int                         setNickname(std::string name);
        bool                        set_op(std::string pass);
        int                         setUsername(std::string name);
        void                        upgradeState();

    private :
        int             id;
        int             client_fd;
        std::string     nickname;
        Client::State   state;
        std::string     username;
        std::string     realname;
        bool            op;
};

#endif