#include <map>
#include "Channel.hpp"
#include "client_irc.hpp"


class channel_map {
    public:
        std::map<int, Client_irc> client_map;
        std::map<std::string, client_map> channel_map;
        

};