/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/16 18:33:32 by akouame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_irc.hpp"
std::map<std::string, Channel> mapchannels;

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

std::map<int, Client_irc> &IrcServer::getMapclient(){
    return this->mapclients;
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

void    IrcServer::setMapclients(const std::map<int, Client_irc> &mapcl){
    this->mapclients = mapcl;
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
        std::map<std::string, Channel>::iterator it;
        for (it = mapchannels.begin(); it != mapchannels.end(); it++)
        {
            if (it->second.clients.find(client.get_nick()) != it->second.clients.end())
                it->second.clients.erase(client.get_nick());
        }
        mapclients.erase(fds[i].fd);
        fds[i] = fds[*numberFd];
        std::memset(&recvbuffer, 0, sizeof(recvbuffer));
        return 0;
    }
    append += std::string(recvbuffer, recvalue);
    if (append.find("\r\n") != std::string::npos)
    {
        client.set_stringtoappend(append);
        client.fd_client = fds[i].fd;
        mapclients[fds[i].fd] = client;
        std::memset(&recvbuffer, 0, sizeof(recvbuffer));
        return 1;
    }
    else
        RecieveIncomingData(numberFd, i);
    return 0;
}

void    IrcServer::RemoveCRLF(int i)
{
    Client_irc &client = getClient(fds[i].fd);
    std::vector<std::string> split;
    int count = countCmd(mapclients[fds[i].fd].get_stringtoappend());
    if (count == 1)
        split.push_back(TrimString(client.get_stringtoappend()));
    else
        SplitString(client.get_stringtoappend(), split, count);
	client.set_commands(split);
    mapclients[fds[i].fd] = client;
}


void    IrcServer::Authentification(int i)
{
    if (mapclients.at(fds[i].fd).get_registered())
    {
        mapclients.at(fds[i].fd).set_commands(split_string(mapclients.at(fds[i].fd).get_commands()[0], ' '));
        display_vct_str(mapclients.at(fds[i].fd).get_commands());
		execute_command(mapclients.at(fds[i].fd).get_commands(), &mapclients.at(fds[i].fd));
    }
    else 
	{
		if (mapclients.at(fds[i].fd).parse_registration((char*)mapclients.at(fds[i].fd).get_commands()[0].c_str(), password, mapclients) == true)// attention
		{
			mapclients.at(fds[i].fd).msg = ":ircserv 001 :" + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n" \
			+ "If you need any help, just ask.\r\n Have a great time! /~ " +\
			mapclients.at(fds[i].fd).get_nick() + " ~/" + "\r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
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
                }
    
            }
        }
    }
    close(sockFd);
    freeaddrinfo(this->result);
}
//--
void    IrcServer::check_Join_cmd(const std::vector<std::string> &command, Client_irc *client)
{
    if (command[1].find(','))
        {
            std::vector<std::string>    chanel_names;
            std::vector<std::string>    chanel_keys;
			
            chanel_names = split_string(command[1], ',');
            chanel_keys = split_string(command[2], ',');
            if (chanel_keys.size() > chanel_names.size())
            {
                client->msg = "Error: to much passwords for a chennl\r\n";
                client->send_msg_to_client();
            }
            else
            {
                for (size_t i = 0; i < chanel_names.size(); i++)
                {
                    int count_exist = mapchannels.count(chanel_names[i]);
                    if (count_exist > 0)
                    {
                        std::cout << "ana d5lt hna 1\n";//Dlt
                        if (mapchannels[chanel_names[i]].get_key() == chanel_keys[i] && mapchannels[chanel_names[i]].get_invite_only() == false)
                        {
                            if (mapchannels[chanel_names[i]].clients.find(client->get_nick()) == mapchannels[chanel_names[i]].clients.end())
                            {
                                mapchannels[chanel_names[i]].clients.insert(std::make_pair(client->get_nick(), client));
                                client->msg = "ircserv :You are join this channel succesfully 1!\r\n";
                                client->send_msg_to_client();
                            }
                            else
                            {
                                client->msg = "ircserv Error: this client is already exist on this channel !\r\n";
                                client->send_msg_to_client();
                            }
                        }
                        else
                        {
                            client->msg = client->error_msg.ERR_PASSWDMISMATCH;
                            client->send_msg_to_client();
                        }
                    }
                    else
                    {
                        Channel chnl(chanel_names[i], client);
                        client->set_operator(true);
                        mapchannels.insert(std::make_pair(chanel_names[i], chnl));
                        client->msg = "ircserv :You are join this channel succesfully 2!\r\n";
                        client->send_msg_to_client();
                        std::cout << "ana d5lt hna 2\n";//Dlt
                        // mapchannels[chanel_names[i]].clients.at(client.get_nick())->set_operator(true);
                        if (!chanel_keys[i].empty())
                        {
                            mapchannels[chanel_names[i]].set_key(chanel_keys[i]);
                            std::cout << "ana d5lt hna 3\n";//Dlt
                        }
                        else
                            std::cout << "ana dkhlt hna 4\n";//Dlt
                    }
                }
            }
        }
}

void IrcServer::execute_command(const std::vector<std::string> &command, Client_irc *client)
{
    //code dial lkosala ach had t5rbi9
    if (command[0] == "JOIN")
        check_Join_cmd(command, client);
        // mapchannels["empty"].join_command(command, client);
    // if (command[0] == "KICK") {
    //     kick_cmd(command, client);
    // } else if (command[0] == "INVITE") {
    //     Channel::cmd_invite(command[1]);
    // } else if (command[0] == "TOPIC") {
    //     Channel::cmd_topic(command[1]);
    // } else if (command[0] == "MODE") 
    // {
    // }
}
