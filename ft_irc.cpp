/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/13 17:34:26 by akouame          ###   ########.fr       */
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

Client_irc &IrcServer::getClient(int fd)
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
    fds[*numberFd].fd = clientFd;
    fds[*numberFd].events = POLLIN;
    *numberFd+= 1;
}

int    IrcServer::RecieveIncomingData(int *numberFd, int i)
{
    Client_irc &client = getClient(fds[i].fd);
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
        mapclients.erase(fds[i].fd);
        fds[i] = fds[*numberFd];
        std::memset(&recvbuffer, 0, sizeof(recvbuffer));
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
    Client_irc &client = getClient(fds[i].fd);
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
        for(size_t i = 0; i < split.size(); i++)
            client.buffer += split[i] + " ";
        client.set_commands(split);
    }
    mapclients[fds[i].fd] = client;
}

void    IrcServer::Authentification(int i)
{
    //Client_irc &clt = getClient(fds[i].fd);
    if (mapclients.at(fds[i].fd).get_registered())
    {
        
            mapclients.at(fds[i].fd).msg = ":irc.1337.com 462  1 " + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n" \
        + "If you need any help, just ask.\r\n Have a great time! /~ " +\
        mapclients.at(fds[i].fd).get_nick() + " ~/" + "\r\n";
        mapclients.at(fds[i].fd).send_msg_to_client();
        // std::cerr << ":You may not reregister" << std::endl;
    }
    else {
        if (mapclients.at(fds[i].fd).get_commands().size() == 1)
        {
            if (mapclients.at(fds[i].fd).parse_registration((char*)mapclients.at(fds[i].fd).get_commands()[0].c_str(), password) == true)// attention
            {
                mapclients.at(fds[i].fd).msg = ":irc.1337.com 462 2 " + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n" \
            + "If you need any help, just ask.\r\n Have a great time! /~ " +\
            mapclients.at(fds[i].fd).get_nick() + " ~/" + "\r\n";
            mapclients.at(fds[i].fd).send_msg_to_client();
            }
        }
        else
        {
            for (size_t i = 0; i < mapclients.at(fds[i].fd).get_commands().size(); i++)
                mapclients.at(fds[i].fd).parse_registration((char*)mapclients.at(fds[i].fd).get_commands()[i].c_str(), password);
            if (mapclients.at(fds[i].fd).get_registered())
           {
        
                mapclients.at(fds[i].fd).msg = ":irc.1337.com 462 3  " + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n" \
                 + "If you need any help, just ask.\r\n Have a great time! /~ " +\
                mapclients.at(fds[i].fd).get_nick() + " ~/" + "\r\n";
                mapclients.at(fds[i].fd).send_msg_to_client();
              // std::cerr << ":You may not reregister" << std::endl;
             }
        }
    }
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
                    Authentification(i);
                  
                    // Client_irc  mapclients.at(fds[i].fd)(fds[i].fd);
                    
                    
                    
                        /////      TO Do     /////////
                    //// function (handle request [buf])
                    //// send reply (connected succesfully)
                    //// trait commands
                    // std::cout << "Recv : " << buf;
                    // std::memset(&buf, 0, sizeof(buf));
                }
    
            }
        }
    }
    close(sockFd);
    freeaddrinfo(this->result);
}
