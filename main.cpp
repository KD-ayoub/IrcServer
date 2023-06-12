/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadi <akadi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 17:53:34 by akadi             #+#    #+#             */
/*   Updated: 2023/06/12 11:52:30 by akadi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "ParsingChannelCommands.hpp"

int main(int ac, char **av)
{

    checkArguments(ac, av);
    IrcServer irc(av[1], av[2]);
    // struct pollfd fds[1024];
    int sockFd = irc.SetupServer();
    /////////////      poll()            ////////////////
    //int maxFd = sockFd;
    irc.AccetConnection(sockFd);
    ////////////       select()         ///////////////
    // fd_set  fdset;

    // FD_ZERO(&fdset);
    // FD_SET(sockFd, &fdset);
    // int max_fd = sockFd;
    // while (true)
    // {
    //     fd_set copyfdset = fdset;
        // int numberFd = select(max_fd + 1, &copyfdset, NULL, NULL, NULL);
    //     std::cout << numberFd << std::endl;
    //     for (int i = 0; i <= max_fd; i++)
    //     {
    //         if (FD_ISSET(i, &copyfdset))
    //         {
    //             if (i == sockFd)
    //             {
    //                 /// New connexion 
    //                 int clientFd;
    //                 if ((clientFd = accept(sockFd, result->ai_addr, &result->ai_addrlen)) == -1)
    //                     return (std::cout << "Error accepting\n", 0);
    //                 std::cout << "Connected...." << std::endl;
    //                 FD_SET(clientFd, &fdset);
    //                 if (clientFd > max_fd)
    //                 {
    //                     max_fd = clientFd;
    //                 }
    //             }
    //             else
    //             {
    //                 //// Reading data from client
    //                 char buf[1024];
    //                 if (recv(i, &buf, sizeof(buf), 0) == 0)
    //                 {
    //                     std::cout << "Disconected....\n";
    //                     close(i);
    //                     FD_CLR(i, &fdset);
    //                     break;
    //                 }
    //                 std::cout << "Recv: " << buf;
    //                 std::memset(&buf, 0, sizeof(buf));
    //             }
    //         }
    //     }
    // }
    ///////////////////////////////////////////////////////


    // while (true)
    // {  
    //     if ((acceptFd = accept(sockFd, result->ai_addr, &result->ai_addrlen)) == -1)
    //         return (std::cout << "Error accepting\n", 0);
    //     std::cout << "Connected...." << std::endl;
    //     while (true)
    //     {
    //         if (recv(acceptFd, buff, sizeof(buff), 0) == 0)
    //             break;
    //         std::cout << "Recv : " << buff;
    //         std::memset(&buff, 0, sizeof(buff));
    //     }
    //     std::cout << "Disconected...." << std::endl;
    //     close(acceptFd);
    // }
    
}