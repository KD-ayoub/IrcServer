/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qabl <yel-qabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/19 20:51:43 by yel-qabl         ###   ########.fr       */
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
    do{
        
    int recvalue = recv(fds[i].fd, &recvbuffer, sizeof(recvbuffer), 0);
    std::cout << fds[i].fd << std::endl;
    if (recvalue == -1)
        Error("Error in recv");
    if (recvalue == 0)
    {
        std::cout << "Disonnected...." << std::endl;
        *numberFd-=1;
        close(fds[i].fd);
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
    } while (append.find("\n") == std::string::npos);
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
			mapclients.at(fds[i].fd).msg = ":ircserv 001 " + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n" \
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
int IrcServer::client_finder(std::string command)
{
    for(std::map<int, Client_irc>::iterator it = mapclients.begin(); it != mapclients.end(); ++it)
    {
        if (it->second.get_nick() == command)
        {
            return (it->second.fd_client);
        }
    }
    return -1;
}

//--
void    IrcServer::kick_command(const std::vector<std::string> &command, Client_irc *client)
{
    if (command.size() < 3)
    {
        client->msg = "Error: KICK <channel> <nick> [comment]\r\n";
        client->send_msg_to_client();
        return ;
    }
    if (mapchannels.find(command[1]) != mapchannels.end())
    {
        for(std::map<std::string, Client_irc*>::iterator it = mapchannels[command[1]].clients.begin(); it != mapchannels[command[1]].clients.end(); it++)
        { 
            if (it->second->get_nick() == command[2])
            {
                // std::cout << "(kick) = " << client->get_operator() << std::endl;
                if (client->get_operator())
                {
                    mapchannels.at(command[1]).clients.erase(it->second->get_nick());
                    client->msg = "Client kicked from channel\r\n";
                    client->send_msg_to_client();
                    it->second->msg = "you have been kicked\r\n";
                    it->second->send_msg_to_client();
                    return ;
                }
                else
                {
                    client->msg = "You are not an operator\r\n";
                    client->send_msg_to_client();
                }
            }
        }
        client->msg = "Error: client not found in this channel\r\n";
        client->send_msg_to_client();
    }
    else
    {
        client->msg = "Error: Channel doesn't exist\r\n";
        client->send_msg_to_client();
    }
}


void    IrcServer::check_Join_cmd(const std::vector<std::string> &command, Client_irc *client)
{
    std::vector<std::string>    chanel_names;
    std::vector<std::string>    chanel_keys;
    
    if (command.size() < 2)
    {
        client->cmd = "JOIN";
        client->msg = client->error_msg.ERR_NEEDMOREPARAMS;
        client->send_msg_to_client();
        return ;
    }
    chanel_names = split_string(command[1], ',');
    if (command.size() > 2)
        chanel_keys = split_string(command[2], ',');
    else
        chanel_keys.push_back("");
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
                if (mapchannels[chanel_names[i]].number_of_users >= mapchannels[chanel_names[i]].user_limit)
                {
                    client->msg = "ircserv 471 :" + chanel_names[i] + " :Cannot join channel (+l)\r\n";
                    client->send_msg_to_client();
                    return ;
                }
                std::cout << "ana d5lt hna 1\n";//Dlt
                if (mapchannels[chanel_names[i]].get_invite_only() == false) 
                {
                    if (mapchannels[chanel_names[i]].get_key() == chanel_keys[i])
                    {
                        if (mapchannels[chanel_names[i]].clients.find(client->get_nick()) == mapchannels[chanel_names[i]].clients.end())
                        {
                                mapchannels[chanel_names[i]].clients.insert(std::make_pair(client->get_nick(), client));
                                mapchannels[chanel_names[i]].number_of_users++;
                                client->msg = "ircserv 001 :You are join this channel succesfully 1!\r\n";
                                client->send_msg_to_client();   
                        }
                        else
                        {
                            std::cout << "mok\n\n";
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
                if (mapchannels[chanel_names[i]].get_invite_only() == true) 
                {
                    bool check = false;
                    for (size_t j = 0; j < mapchannels[chanel_names[i]].get_invited_user().size(); j++)
                    {
                        if (client->get_nick() == mapchannels[chanel_names[i]].get_invited_user()[j])
                            check = true;
                    }
                    if (check == true)
                    {
                        if (mapchannels[chanel_names[i]].clients.find(client->get_nick()) == mapchannels[chanel_names[i]].clients.end())
                        {
                            mapchannels[chanel_names[i]].clients.insert(std::make_pair(client->get_nick(), client));
                            mapchannels[chanel_names[i]].number_of_users++;
                            client->msg = "ircserv :You are join this channel succesfully 2!\r\n";
                            client->send_msg_to_client();
                        }
                        else
                        {
                            client->msg = "ircserv Error: this client is already exist on this channel !\r\n";
                            client->send_msg_to_client();
                        }
                    }
                    if (check == false)
                    {
                        client->msg = "ircserv 473 :" + chanel_names[i] + " :Cannot join channel (+i)\r\n";
                        client->send_msg_to_client();
                    }
                }
            }
            else
            {
                if (chanel_names[i][0] != '#')
                {
                    client->msg = "ircserv 403 " + chanel_names[i] + " :No such channel\r\n";
                    client->send_msg_to_client();
                    return ; 
                }
                Channel chnl(chanel_names[i], client);
                std::cout << "-------\n";
                std::cout << chnl.clients.at(client->get_nick())->get_nick() << std::endl;
                std::cout << "-------\n";
                chnl.clients.at(client->get_nick())->set_operator(true);
                mapchannels.insert(std::make_pair(chanel_names[i], chnl));
                mapchannels[chanel_names[i]].number_of_users++;
                mapchannels[chanel_names[i]].operators.push_back(client->get_nick());
                client->msg = "ircserv :You are join this channel succesfully 3!\r\n";
                client->send_msg_to_client();
                // mapchannels[chanel_names[i]].clients.at(client.get_nick())->set_operator(true);
                std::cout << "ana d5lt hna 2\n";//Dlt
                std::cout << "chanel_name = " << chanel_names[i] << std::endl;
                std::cout << "chanel_key = " << chanel_keys[i] << std::endl;
                
                mapchannels[chanel_names[i]].set_key(chanel_keys[i]);
            }
        
        }
    }   
}


void    IrcServer::check_Invite_cmd(const std::vector<std::string> &command, Client_irc *client)
{
     if (command.size() < 3)
        {
            client->msg = "Error: INVITE command requires 2 arguments\r\n";
            client->send_msg_to_client();
        }
        // the following condition is to check if its an operator
        else if (!mapchannels[command[2]].is_operator(client->get_nick())) // command[2]
        {
            client->msg = "Error: you are not an operator\r\n";
            client->send_msg_to_client();
        }
        else
        {
            if (mapchannels.find(command[2]) == mapchannels.end())
            {
                client->msg = "Error: channel doesn't exist\r\n";
                client->send_msg_to_client();
            }
            else
            {
                const std::vector<std::string> &invitedUsers = mapchannels[command[2]].get_invited_user();
                bool isInvited = false;
                for (std::vector<std::string>::const_iterator it = invitedUsers.begin(); it != invitedUsers.end(); ++it)
                {
                       if (*it == client->get_nick())
                       {
                            isInvited = true;
                            break;
                       }
                }

                if (mapchannels[command[2]].get_invite_only() && !isInvited) // if channel is invite only and user is not invited
                {
                       client->msg = "Error: you are not invited to this channel\r\n";
                       client->send_msg_to_client();
                }
                else
                {
                       if (mapclients.find(client->fd_client) == mapclients.end()) // if user doesn't exist
                       {
                            client->msg = "Error: user doesn't exist\r\n";
                            client->send_msg_to_client();
                       }
                       else // if user is invited to channel and user exists send invite message to user
                       {
                            mapchannels[command[2]].cmd_invite(command[1]); // this is the new version
                            std::string message = ":" + client->get_nick() + " INVITE " + command[2] + " " + command[1] + "\r\n";
                            mapclients[client_finder(command[1])].msg = message;
                            mapclients[client_finder(command[1])].send_msg_to_client();
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
    else if (command[0] == "INVITE")
        check_Invite_cmd(command, client);
    else if (command[0] == "KICK")
        kick_command(command, client);
    /*##############################################################################*/

    else if(command[0] == "TOPIC") // TOPIC <channel> [<topic>]
    {
        if(command.size() == 2)
        {
            if(mapchannels.find(command[1]) == mapchannels.end())
            {
                client->msg = "Error: channel doesn't exist\r\n";
                client->send_msg_to_client();
            }
            else
            {
                if(mapchannels[command[1]].get_op_topic() && !mapchannels[command[1]].is_operator(client->get_nick()))
                {
                    client->msg = "Error: you are not an operator\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    client->msg = ":" + client->get_nick() + " TOPIC " + command[1] + " :" + mapchannels[command[1]].get_topic() + "\r\n";
                    client->send_msg_to_client();
                }
            }
        }
        else if(command.size() == 3)
        {
            if(mapchannels.find(command[1]) == mapchannels.end())
            {
                client->msg = "Error: channel doesn't exist\r\n";
                client->send_msg_to_client();
            }
            else
            {
                if(mapchannels[command[1]].get_op_topic() && !mapchannels[command[1]].is_operator(client->get_nick()))
                {
                    client->msg = "Error: you are not an operator\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    mapchannels[command[1]].set_topic(command[2]);
                    client->msg = ":" + client->get_nick() + " TOPIC " + command[1] + " :" + mapchannels[command[1]].get_topic() + "\r\n";
                    client->send_msg_to_client();
                }
            }
        }
        else
        {
            client->msg = "Error: TOPIC command requires 1 or 2 arguments\r\n";
            client->send_msg_to_client();
        }
    }
    
/*#########################################################################################*/
    
else if (command[0] == "MODE")
{
    if (command.size() < 3)
    {
        client->msg = "Error: MODE command requires 2 arguments\r\n";
        client->send_msg_to_client();
    }
    else if (mapchannels.find(command[1]) == mapchannels.end())
    {
        client->msg = "Error: channel doesn't exist\r\n";
        client->send_msg_to_client();
    }
    else if (!mapchannels[command[1]].is_operator(client->get_nick()))
    {
        client->msg = "Error: you are not an operator\r\n";
        client->send_msg_to_client();
    }
    else
    {
        std::string mode = command[2];
        bool addMode = true;
        std::string::iterator it = mode.begin();
        while (it != mode.end())
        {
            if (*it == '+')
            {
                addMode = true;
            }
            else if (*it == '-')
            {
                addMode = false;
            }
            else if (*it == 'o')
            {
                if (addMode)
                {
                    mapchannels[command[1]].add_operator(command[3]);
                }
                else
                {
                    mapchannels[command[1]].remove_operator(command[3]);
                }
            }
            else if (*it == 'i')
            {
                if (addMode)
                    mapchannels[command[1]].change_invite("+");
                    
                else
                    mapchannels[command[1]].change_invite("-");
            }
            else if (*it == 't')
            {
                if (addMode)
                    mapchannels[command[1]].change_optopic("+");
                else
                    mapchannels[command[1]].change_optopic("-");
            }
            else if (*it == 'l')
            {
                if (addMode)
                {
                    mapchannels[command[1]].change_userlimits("+", std::stoi(command[3]) );
                }
                else
                {
                    mapchannels[command[1]].change_userlimits("-", std::stoi(command[3]));
                }
            }
            else if (*it == 'k')
            {
                if (addMode)
                {
                    mapchannels[command[1]].change_password("+", command[3]);
                }
                else
                {
                    mapchannels[command[1]].change_password("-", command[3]);
                }
            }
            else
            {
                client->msg = "Error: invalid mode\r\n";
                client->send_msg_to_client();
                return;
            }
            ++it;
        }
        std::string message = ":" + client->get_nick() + " MODE " + command[1] + " " + command[2] + "\r\n";
        mapchannels[command[1]].broadcast(message, client->fd_client);
    }
}

/*####################################################################################*/



else if (command[0] == "PART") 
{
    if (command.size() < 2)
    {
        client->msg = "Error: PART command requires 1 argument\r\n";
        client->send_msg_to_client();
    }
    else if (mapchannels.find(command[1]) == mapchannels.end())
    {
        client->msg = "Error: channel doesn't exist\r\n";
        client->send_msg_to_client();
    }
    else
    {
        if (mapchannels[command[1]].clients.find(client->get_nick()) == mapchannels[command[1]].clients.end())
        {
            client->msg = "Error: you are not in this channel\r\n";
            client->send_msg_to_client();
        }
        else
        {
            mapchannels[command[1]].clients.erase(client->get_nick());
            std::string message = ":" + client->get_nick() + " PART " + command[1] + "\r\n";
            mapchannels[command[1]].broadcast(message, client->fd_client);
        }
    }
}


else if (command[0] == "NAMES")
{
    if (command.size() < 2)
    {
        client->msg = "Error: NAMES command requires 1 argument\r\n";
        client->send_msg_to_client();
    }
    else if (mapchannels.find(command[1]) == mapchannels.end())
    {
        client->msg = "Error: channel doesn't exist\r\n";
        client->send_msg_to_client();
    }
    else
    {
        std::string message = ":" + client->get_nick() + " NAMES " + command[1] + " :";
        std::map<std::string, Client_irc*>::iterator it;
        for (it = mapchannels[command[1]].clients.begin(); it != mapchannels[command[1]].clients.end(); ++it)
        {
            message += it->first + " ";
        }
        message += "\r\n";
        client->msg = message;
        client->send_msg_to_client();
    }
}

else if (command[0] == "LIST")
{
    if (command.size() == 1)
    {
        std::map<std::string, Channel>::iterator it;
        for (it = mapchannels.begin(); it != mapchannels.end(); ++it)
        {
            std::string message = ":" + client->get_nick() + " LIST " + it->first + " " + std::to_string(it->second.number_of_users) + " :";
            message += it->second.get_topic() + "\r\n";
            client->msg = message;
            client->send_msg_to_client();
        }
    }
    else if (command.size() == 2)
    {
        if (mapchannels.find(command[1]) == mapchannels.end())
        {
            client->msg = "Error: channel doesn't exist\r\n";
            client->send_msg_to_client();
        }
        else
        {
            std::string message = ":" + client->get_nick() + " LIST " + command[1] + " " + std::to_string(mapchannels[command[1]].number_of_users) + " :";
            message += mapchannels[command[1]].get_topic() + "\r\n";
            client->msg = message;
            client->send_msg_to_client();
        }
    }
    else if (command.size() == 3)
    {
        if (mapchannels.find(command[1]) == mapchannels.end())
        {
            client->msg = "Error: channel doesn't exist\r\n";
            client->send_msg_to_client();
        }
        else
        {
            std::string message = ":" + client->get_nick() + " LIST " + command[1] + " " + std::to_string(mapchannels[command[1]].number_of_users) + " :";
            message += mapchannels[command[1]].get_topic() + "\r\n";
            client->msg = message;
            client->send_msg_to_client();
        }
    }
    else
    {
        client->msg = "Error: LIST command requires 0, 1 or 2 arguments\r\n";
        client->send_msg_to_client();
    }
}

/*-------------------------- privmsg ------------------------------*/

else if (command[0] == "PRIVMSG") // PRIVMSG <receiver>{,<receiver>} <text to be sent>
{
    if (command.size() < 3)
    {
        client->msg = "Error: PRIVMSG command requires 2 arguments\r\n";
        client->send_msg_to_client();
    }
    else
    {
        std::vector<std::string> receivers = split_string(command[1], ',');
        for (size_t i = 0; i < receivers.size(); i++)
        {
            if (receivers[i][0] == '#')
            {
                if (mapchannels.find(receivers[i]) == mapchannels.end())
                {
                    client->msg = "Error: channel doesn't exist\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    if (mapchannels[receivers[i]].clients.find(client->get_nick()) == mapchannels[receivers[i]].clients.end())
                    {
                        client->msg = "Error: you are not in this channel\r\n";
                        client->send_msg_to_client();
                    }
                    else
                    {
                        std::string message = ":" + client->get_nick() + " PRIVMSG " + receivers[i] + " :" + command[2];
                    for (size_t j = 3; j < command.size(); j++)
                    {
                        message += " " + command[j] ;
                    }
                    message += "\r\n";
                    std::cout << "-----------" <<message << std::endl;
                        mapchannels[receivers[i]].broadcast(message, client->fd_client);
                    }
                }
            }
            else
            {
                if (mapclients.find(client_finder(receivers[i])) == mapclients.end())
                {
                    client->msg = "Error: user doesn't exist\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    std::string message = ":" + client->get_nick() + " PRIVMSG " + receivers[i] + " :" + command[2] ;
                    for (size_t j = 3; j < command.size(); j++)
                    {
                        message += " " + command[j] ;
                    }
                    message += "\r\n";
                    mapclients[client_finder(receivers[i])].msg = message;
                    mapclients[client_finder(receivers[i])].send_msg_to_client();
                }
            }
        }
    }
}


/*-------------------------- BOT ------------------------------*/
// still on work

if (command[0] == "BOT")
{
        
        client->msg = "welcome to BOT of definition. . .\n***   Enter a word you want to define:  DEFINE <word> \n***   (exit) to return to server\n";
        client->send_msg_to_client();

}


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
