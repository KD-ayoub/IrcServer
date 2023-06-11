/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 17:53:49 by akadi             #+#    #+#             */
/*   Updated: 2023/06/10 11:43:47 by akadi            ###   ########.fr       */
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

class IrcServer
{
    private :
        std::string password;
        std::string port;
        struct pollfd fds[1024];
        struct addrinfo hints;
        struct addrinfo *result;
    public :
        IrcServer();
        ~IrcServer();
        IrcServer(std::string port, std::string pass);

        std::string getPort() const ;
        std::string  getPassword() const ;

        void    setPort(std::string port);
        void    setPassword(std::string pass);

        int    SetupServer();
        void    AccetConnection(int sockFd);
          
};

bool checkPort(char *str);
void    Error(const char *str);
void    checkArguments(int ac, char **av);


#endif