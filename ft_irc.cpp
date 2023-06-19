/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouame <akouame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 18:03:29 by akadi             #+#    #+#             */
/*   Updated: 2023/06/19 18:08:25 by akouame          ###   ########.fr       */
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
			mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick()+" :Welcome to Our IRC Server!\r\n"; \
			mapclients.at(fds[i].fd).send_msg_to_client();
			mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() + " :Your hostname is " + getMachineHost() + "\r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" :     ______        _             \r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" :    / ____/___    (_)___  __  __ \r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" :   / __/ / __ \\  / / __ \\/ / / / \r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" :  / /___/ / / / / / /_/ / /_/ /  \r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" : /_____/_/ /_/_/ /\\____/\\__, /   \r\n";
			mapclients.at(fds[i].fd).send_msg_to_client();
            mapclients.at(fds[i].fd).msg = ":" + getMachineHost() + " 001 " + mapclients.at(fds[i].fd).get_nick() +" :            /___/      /____/    \r\n";
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
        client->cmd = "KICK";
        client->msg = client->error_msg.ERR_NEEDMOREPARAMS;
        client->send_msg_to_client();
        // client->msg = ":" + getMachineHost() + " 001 " + client->get_nick()+ " :KICK :Not enough parameters <channel> <nick> [comment]\r\n";
        // client->send_msg_to_client();
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
                    client->msg = ":" + getMachineHost() + " 001 " + client->get_nick() + " :KICK " + it->second->get_nick() + " from " + command[1] + "\r\n";
                    client->send_msg_to_client();
                    it->second->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :You have been kicked\r\n"; /*!!////////!!!!!!!!! segmentationfault sometimes /////////////////*/
                    it->second->send_msg_to_client();
                    mapchannels.at(command[1]).clients.erase(it->second->get_nick());
                    return ;
                }
                else if (mapchannels[command[1]].clients.find(client->get_nick()) == mapchannels[command[1]].clients.end())
                {
                    client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :You are not in this channel\r\n";
                    client->send_msg_to_client();
                    return ;
                }
                else
                {
                    client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :You are not an operator\r\n";
                    client->send_msg_to_client();
                    return ;
                }
            }
        }
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :client not found in this channel\r\n";
        client->send_msg_to_client();
    }
    else
    {
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :Channel doesn't exist\r\n";
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
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :too much passwords for a channel\r\n";
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
                    client->msg =  ":" + getMachineHost() + " 471 " + client->get_nick() + " :" + chanel_names[i] + " Cannot join channel (+l)\r\n";
                    client->send_msg_to_client();
                    return ;
                }
                // std::cout << "ana d5lt hna 1\n";//Dlt
                if (mapchannels[chanel_names[i]].get_invite_only() == false) 
                {
                    if (mapchannels[chanel_names[i]].get_key() == chanel_keys[i])
                    {
                        if (mapchannels[chanel_names[i]].clients.find(client->get_nick()) == mapchannels[chanel_names[i]].clients.end())
                        {
                            mapchannels[chanel_names[i]].clients.insert(std::make_pair(client->get_nick(), client));
                            mapchannels[chanel_names[i]].number_of_users++;
                            client->msg = ":" + client->get_nick() + "!" + client->get_user().username + "@" + getMachineHost() + " JOIN " + chanel_names[i] + "\r\n";
                                //"ircserv 001 :You are join this channel succesfully 1!\r\n";
                            client->send_msg_to_client();
                            mapchannels[chanel_names[i]].broadcast(client->msg, client->fd_client);
                            client->msg = ":" + getMachineHost() + " 353 " + client->get_nick() + " @ " + chanel_names[i] + " " + getChannelUsers(chanel_names[i]) + "\r\n";
                            client->send_msg_to_client();
                            client->msg = ":" + getMachineHost() + " 366 " + client->get_nick() + " " + chanel_names[i] + " :End of /NAMES list.\r\n";
                            client->send_msg_to_client();
                        }
                        else
                        {
                            // std::cout << "mok\n\n";
                            client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :this client is already exist on this channel !\r\n";
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
                            client->msg = ":" + client->get_nick() + "!" + client->get_user().username + "@" + getMachineHost() + " JOIN " + chanel_names[i] + "\r\n";
                            client->send_msg_to_client();
                             //"ircserv :You are join this channel succesfully 2!\r\n";
                            mapchannels[chanel_names[i]].broadcast(client->msg,  client->fd_client);
                            client->msg = ":" + getMachineHost() + " 353 " + client->get_nick() + " @ " + chanel_names[i] + " " + getChannelUsers(chanel_names[i]) + "\r\n";
                            client->send_msg_to_client();
                            client->msg = ":" + getMachineHost() + " 366 " + client->get_nick() + " " + chanel_names[i] + " :End of /NAMES list.\r\n";
                            client->send_msg_to_client();

                        }
                        else
                        {
                            client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :this client is already exist on this channel !\r\n";
                            client->send_msg_to_client();
                        }
                    }
                    if (check == false)
                    {
                        client->msg = ":" + getMachineHost() + " 473 " + client->get_nick() + " :" + chanel_names[i] + " Cannot join channel (+i)\r\n";
                        client->send_msg_to_client();
                    }
                }
            }
            else
            {
                if (chanel_names[i][0] != '#' || (chanel_names[i][0] == '#' && chanel_names[i][1] == '\0'))
                {
                    client->msg = ":" + getMachineHost() + " 403 " + client->get_nick() + " :" + chanel_names[i] + " No such channel\r\n";
                    client->send_msg_to_client();
                    return ; 
                }
                Channel chnl(chanel_names[i], client);
                std::cout << "-------\n";
                std::cout << chnl.clients.at(client->get_nick())->get_nick() << std::endl;
                std::cout << "-------\n";
                chnl.clients.at(client->get_nick())->set_operator(true);
                mapchannels.insert(std::make_pair(chanel_names[i], chnl));
                // mapchannels[chanel_names[i]]
                mapchannels[chanel_names[i]].number_of_users++;
                mapchannels[chanel_names[i]].operators.push_back(client->get_nick());
                client->msg = ":" + client->get_nick() + "!" + client->get_user().username + "@" + getMachineHost() + " JOIN " + chanel_names[i] + "\r\n";
                client->send_msg_to_client();
                client->msg = ":" + getMachineHost() + " 353 " + client->get_nick() + " = " + chanel_names[i] + " :@" + client->get_nick() + "\r\n";
                client->send_msg_to_client();
                client->msg = ":" + getMachineHost() + " 366 " + client->get_nick() + " " + chanel_names[i] + " :End of /NAMES list.\r\n";
                client->send_msg_to_client();
                // :irc.example.com MODE #test +nt
                // :irc.example.com 353 dan = #test :@dan
                // :irc.example.com 366 dan #test :End of /NAMES list.
                //client->msg = "ircserv :You are join this channel succesfully 3!\r\n";
                // client->send_msg_to_client();
                // mapchannels[chanel_names[i]].clients.at(client.get_nick())->set_operator(true);
                std::cout << "ana d5lt hna 2\n";//Dlt
                std::cout << "chanel_name = " << chanel_names[i] << std::endl;
                std::cout << "chanel_key = " << chanel_keys[i] << std::endl;
                
                mapchannels[chanel_names[i]].set_key(chanel_keys[i]);
            }
        
        }
    }   
}

std::string IrcServer::getChannelUsers(std::string channelname)
{
    std::string userlist = ":";
    std::string useroperator = "";
    std::map<std::string, Client_irc*>::iterator it = mapchannels[channelname].clients.begin();
    while(it != mapchannels[channelname].clients.end())
    {
        if (it->second->get_operator())
            useroperator += it->second->get_nick();
        else
            userlist += it->second->get_nick() + " ";
        it++;
    }
    if (!userlist.empty())
        userlist += "@" + useroperator;
    return userlist;
}


void    IrcServer::check_Invite_cmd(const std::vector<std::string> &command, Client_irc *client)
{
     if (command.size() < 3)
        {
            client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :INVITE command requires 2 arguments\r\n";
            client->send_msg_to_client();
        }
        // the following condition is to check if its an operator
        else if (!mapchannels[command[2]].is_operator(client->get_nick())) // command[2]
        {
            client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :you are not an operator\r\n";
            client->send_msg_to_client();
        }
        else
        {
            if (mapchannels.find(command[2]) == mapchannels.end())
            {
                client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :channel doesn't exist\r\n";
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
                       client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :you are not invited to this channel\r\n";
                       client->send_msg_to_client();
                }
                else
                {
                       if (mapclients.find(client->fd_client) == mapclients.end()) // if user doesn't exist
                       {
                            client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :user doesn't exist\r\n";
                            client->send_msg_to_client();
                       }
                       else // if user is invited to channel and user exists send invite message to user
                       {
                            mapchannels[command[2]].cmd_invite(command[1]); // this is the new version
                            std::string message = ":" + getMachineHost() + " 001 " + client->get_nick() + " INVITE " + command[2] + " " + command[1] + "\r\n";
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
                client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :channel doesn't exist\r\n";
                client->send_msg_to_client();
            }
            else
            {
                if(mapchannels[command[1]].get_op_topic() && !mapchannels[command[1]].is_operator(client->get_nick()))
                {
                    client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :you are not an operator\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    client->msg = ":" + getMachineHost() + " 001 " + client->get_nick() + " TOPIC " + command[1] + " :" + mapchannels[command[1]].get_topic() + "\r\n";
                    client->send_msg_to_client();
                }
            }
        }
        else if(command.size() == 3)
        {
            if(mapchannels.find(command[1]) == mapchannels.end())
            {
                client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :channel doesn't exist\r\n";
                client->send_msg_to_client();
            }
            else
            {
                if(mapchannels[command[1]].get_op_topic() && !mapchannels[command[1]].is_operator(client->get_nick()))
                {
                    client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :you are not an operator\r\n";
                    client->send_msg_to_client();
                }
                else
                {
                    mapchannels[command[1]].set_topic(command[2]);
                    client->msg = ":" + getMachineHost() + " 001 " + client->get_nick() + " TOPIC " + command[1] + " :" + mapchannels[command[1]].get_topic() + "\r\n";
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
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :MODE command requires 2 arguments\r\n";
        client->send_msg_to_client();
    }
    else if (mapchannels.find(command[1]) == mapchannels.end())
    {
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :channel doesn't exist\r\n";
        client->send_msg_to_client();
    }
    else if (!mapchannels[command[1]].is_operator(client->get_nick()))
    {
        client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :you are not an operator\r\n";
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
                client->msg = ":" + getMachineHost() + " 400 " + client->get_nick() + " :invalid mode\r\n";
                client->send_msg_to_client();
                return;
            }
            ++it;
        }
        std::string message = ":" + getMachineHost() + " 001 " + client->get_nick() + " MODE " + command[1] + " " + command[2] + "\r\n";
        mapchannels[command[1]].broadcast(message, client->fd_client);
    }
}
else if (command[0] == "LIST")
{
    std::map<std::string, Client_irc*>::iterator it = mapchannels[command[1]].clients.begin();
    while (it != mapchannels[command[1]].clients.end())
    {
        client->msg = it->second->get_nick() + "\r\n";
        client->send_msg_to_client();
        it++;
    }
}
else
{
    client->msg = ":" + getMachineHost() + " 421 * " + " :" +command[0]+ " Unknown command\r\n";
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
