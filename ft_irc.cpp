/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/12 11:58:21 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "ParsingChannelCommands.hpp"

IrcServer::IrcServer()
{
    this->port = "0";
    this->password = "default";
}

IrcServer::IrcServer(std::string port, std::string pass)
{
    this->port = port;
    this->password = pass;
}

IrcServer::~IrcServer()
{

}

std::string IrcServer::getPort() const
{
    return this->port;
}

std::string IrcServer::getPassword() const
{
    return this->password;
}

void    IrcServer::setPort(std::string port) 
{
    this->port = port;
}

void    IrcServer::setPassword(std::string pass)
{
    this->password = pass;
}

int    IrcServer::SetupServer()
{
    int sockFd, sockoptValue = 1;
    // struct addrinfo hints;
    // struct addrinfo *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo("0.0.0.0", this->port.c_str(), &this->hints, &this->result) != 0)
    {
        std::cout << gai_strerror(8) << std::endl;
        Error("Error getting infos");
    }
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Error ("Error creating socket");
    std::cout << "FD = " << sockFd << std::endl;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &sockoptValue, sizeof(int)) == -1)
        Error ("Error in setsockopt()");
    if (bind(sockFd, this->result->ai_addr, this->result->ai_addrlen) == -1)
        Error ("Error binding socket");
    if (listen(sockFd, SOMAXCONN) == -1)
        Error ("Error listening");
    return sockFd;
}

void    IrcServer::AccetConnection(int sockFd)
{
    ParsingChannelCommands parser;
    int polfd;
    int numberFd = 1;
    std::memset(&this->fds, 0, sizeof(this->fds));
    fds[0].fd = sockFd;
    fds[0].events = POLLIN;
    std::string Appendbuffer;
    while (true)
    {
        polfd = poll(fds, numberFd, -1);
        if (polfd < 0)
            Error("Error in Poll");
        for(int i = 0; i < numberFd; i++)
        {
            if (fds[i].revents && POLLIN)
            {
                if (fds[i].fd == sockFd)
                {
                    //// accept new connexion
                    int clientFd = accept(sockFd, result->ai_addr, &result->ai_addrlen);
                    if (clientFd < 0)
                        Error("Error in accept");
                    std::cout << "Connected...." << std::endl;
                    std::cout << "client entered\n";
                    fds[numberFd].fd = clientFd;
                    fds[numberFd].events = POLLIN;
                    numberFd++;
                }
                else
                {
                    /// incoming data for existing connexion
                    //std::cout << "num : " << numberFd << std::endl;
                    char recvbuffer[512];
                    int recvalue = recv(fds[i].fd, &recvbuffer, sizeof(recvbuffer), 0);
                    std::cout << fds[i].fd << std::endl;
                    if (recvalue == -1)
                        Error("Error in recv");
                    if (recvalue == 0)
                    {
                        std::cout << "Disonnected...." << std::endl;
                        close(fds[i].fd);
                        numberFd--;
                        fds[i] = fds[numberFd];
                        std::memset(&recvbuffer, 0, sizeof(recvbuffer));
                        break;
                    }
                        /////      TO Do     /////////
                    //// function (handle request [buf])
                    //// send reply (connected succesfully)
                    Appendbuffer += std::string(recvbuffer, recvalue);
                    if (Appendbuffer.find("\n") != std::string::npos) {
                        parser.ParseCmd(&Appendbuffer[0], Appendbuffer.length());
                        Appendbuffer.clear();
                    }
                    //send(fds[i].fd, buf, recvalue, 0);
                    //// trait commands
                    //std::cout << "Recv : " << buf;
                    std::memset(&recvbuffer, 0, sizeof(recvbuffer));
                }
            }
        }
    }
    close(sockFd);
    freeaddrinfo(this->result);
}
