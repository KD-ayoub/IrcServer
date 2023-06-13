/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/13 13:05:04 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

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

Client_irc IrcServer::getClient(int fd)
{
    return this->mapclients.at(fd);
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

void    IrcServer::InitPollfd(int sockFd)
{
    std::memset(&this->fds, 0, sizeof(this->fds));
    fds[0].fd = sockFd;
    fds[0].events = POLLIN;
}

void    IrcServer::AcceptNewConnection(int sockFd, int *numberFd)
{
    //// accept new connexion
    int clientFd = accept(sockFd, result->ai_addr, &result->ai_addrlen);
    if (clientFd < 0)
        Error("Error in accept");
    std::cout << "Connected...." << std::endl;
    std::cout << "client entered\n";
    mapclients[clientFd] = Client_irc();
    //Appendbuffer[clientFd] = "";  //// initialize new buffer for this client
    fds[*numberFd].fd = clientFd;
    fds[*numberFd].events = POLLIN;
    *numberFd+= 1;
}

int    IrcServer::RecieveIncomingData(int *numberFd, int i)
{
    Client_irc client;
    std::string append;
    char recvbuffer[512];
    int recvalue = recv(fds[i].fd, &recvbuffer, sizeof(recvbuffer), 0);
    std::cout << fds[i].fd << std::endl;
    if (recvalue == -1)
        Error("Error in recv");
    if (recvalue == 0)
    {
        std::cout << "Disonnected...." << std::endl;
        close(fds[i].fd);
        *numberFd-=1;
        fds[i] = fds[*numberFd];
        std::memset(&recvbuffer, 0, sizeof(recvbuffer));
        mapclients.erase(fds[i].fd);
        //Appendbuffer.erase(fds[i].fd); /// remove buffer for this client
        return 0;
    }
    append += std::string(recvbuffer, recvalue);
    client.set_stringtoappend(append);
    client.fd_client = fds[i].fd;
    mapclients[fds[i].fd] = client;
    std::memset(&recvbuffer, 0, sizeof(recvbuffer));
    return 1;
}

void    IrcServer::RemoveCRLF(int i)
{
    Client_irc client = getClient(fds[i].fd);
    std::vector<std::string> split;
    int count = countCmd(mapclients[fds[i].fd].get_stringtoappend());
    if (count == 1)
    {
        split.push_back(TrimString(client.get_stringtoappend()));
        client.set_commands(split);
    }
    else
    {
        SplitString(client.get_stringtoappend(), split, count);
        client.set_commands(split);
    }
    mapclients[fds[i].fd] = client;
}

void    IrcServer::RunServer(int sockFd)
{
    ParsingChannelCommands parser;
    int numberFd = 1;
    int polfdreturned;
    InitPollfd(sockFd);
    while (true)
    {
        polfdreturned = poll(fds, numberFd, -1);
        if (polfdreturned < 0)
            Error("Error in Poll");
        for(int i = 0; i < numberFd; i++)
        {
            if (fds[i].revents && POLLIN)
            {
                if (fds[i].fd == sockFd)
                    AcceptNewConnection(sockFd, &numberFd);
                else if (!RecieveIncomingData(&numberFd, i))
                    break;
                else
                {
                    RemoveCRLF(i);
                    std::cout << mapclients[fds[i].fd].get_stringtoappend();
                }
                    /// incoming data for existing connexion
                    //std::cout << "num : " << numberFd << std::endl;
                    // Client_irc  clt(fds[i].fd);
                    
                    // if (clt.get_registred())
                    // {
                        
                    //             std::string msg = ":irc.1337.com 462 " + clt.get_nick()+" :Welcome to Our IRC Server!\r\n" + "If you need any help, just ask.\r\n Have a great time! /~ " +
                    // clt.get_nick() + " ~/" + "\r\n";
                    // clt.send_msg_to_client(msg.c_str());
                    //    // std::cerr << ":You may not reregister" << std::endl;
                    // }
                    // else {
                    //     if (clt.parse_registration(recvbuffer, password))
                    //         {
                    //             std::string msg = ":irc.1337.com 001 " + clt.get_nick()+" :Welcome to Our IRC Server!\r\n" + "If you need any help, just ask.\r\n Have a great time! /~ " +
                    // clt.get_nick() + " ~/" + "\r\n";
                    // clt.send_msg_to_client(msg.c_str());
                    //         }
                        
                    // }
                    //     /////      TO Do     /////////
                    // //// function (handle request [buf])
                    // //// send reply (connected succesfully)
                    // Appendbuffer[fds[i].fd] += std::string(recvbuffer, recvalue);
                    // if (Appendbuffer[fds[i].fd].find("\n") != std::string::npos) {
                    //     parser.ParseCmd(Appendbuffer[fds[i].fd].c_str(), Appendbuffer[fds[i].fd].length());
                    //     //Appendbuffer.clear();
                    // }
                    
                    //send(fds[i].fd, buf, recvalue, 0);
                    //// trait commands
                    //std::cout << "Recv : " << buf;
                    
            }
        }
    }
    close(sockFd);
    freeaddrinfo(this->result);
}
