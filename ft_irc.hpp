/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 17:53:49 by akadi             #+#    #+#             */
/*   Updated: 2023/06/13 17:01:45 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC
#define FT_IRC

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>  // getaddrinfo()
#include <unistd.h>
#include <arpa/inet.h> //htons()
#include <sys/select.h> // select()
#include <sys/poll.h> // poll()
#include "ParsingChannelCommands.hpp"
#include "client_irc.hpp"

class Client_irc;

class IrcServer
{
    private :
        std::string password;
        std::string port;
        struct pollfd fds[1024];
        struct addrinfo hints;
        struct addrinfo *result;
        std::map<int, Client_irc> mapclients;
    public :
        IrcServer();
        ~IrcServer();
        IrcServer(std::string port, std::string pass);

        std::string getPort() const ;
        std::string  getPassword() const ;
        Client_irc  &getClient(int);

        void    setPort(std::string port);
        void    setPassword(std::string pass);
        //// server ///////
        int     SetupServer();
        void    RunServer(int);
        void    InitPollfd(int);
        void    AcceptNewConnection(int , int *);
        int     RecieveIncomingData(int *, int);
        void    Authentification(int);
        /////////////////////
        void    RemoveCRLF(int);
        
          
};

bool    checkPort(char *str);
void    Error(const char *str);
void    checkArguments(int ac, char **av);


#endif