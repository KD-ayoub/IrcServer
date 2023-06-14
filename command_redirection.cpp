#include <Channel.hpp>
#include <Client.hpp>

std::map<std::string, Channel*> channel_list;

int join_command(std::vector<std::string> const &command, Client &client) 
{
    if (channel_list.find(command[1]) == channel_list.end()) // if channel doesn't exist
    {
        Channel *new_channel = new Channel(command[1], client);
        channel_list.insert(std::pair<std::string, Channel*>(command[1], new_channel));
        std::cout << "Channel " << command[1] << " created" << std::endl;
        std::cout << "Client " << client.getNickname() << " is now operator of " << command[1] << std::endl;
        return(0);
    }
    else
    {
        channel_list[command[1]]->connect(client);
    }
}

int kick_cmd(std::vector<std::string> const &command, Client &client) 
{
    std::string client_channel = client.get_channel();
    if (channel_list.find(client_channel) == channel_list.end()) // if channel doesn't exist
    {
        std::cout << "can't KICK "<< client.getNickname() << " from this channel: " << std::endl;
        return(0);
    }
    else
    {
        channel_list[client_channel].erase(client.getNickname());
    }
}

bool can_execute(const std::string& command) {
    bool ret = false;
    if (command == "KICK" || command == "INVITE" || command == "TOPIC" || command == "MODE") {
        ret = true;
    }
    return ret;
}


void execute_command(const std::vector<std::string> &command, Client &client) {
    if (command[0] == "JOIN")
    {
        join_command(command, client);
    }
    if (command[0] == "KICK") {
        kick_cmd(command, client);
    } else if (command[0] == "INVITE") {
        Channel::cmd_invite(command[1]);
    } else if (command[0] == "TOPIC") {
        Channel::cmd_topic(command[1]);
    } else if (command[0] == "MODE") 
    {
        if (command.size() == 1)
            Channel::list_cmd();
        else

    }
}