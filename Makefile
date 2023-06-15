# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akouame <akouame@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/13 16:11:29 by akouame           #+#    #+#              #
#    Updated: 2023/06/15 19:57:58 by akouame          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

RM = rm -rf

src = main.cpp ft_irc.cpp checkArguments.cpp outils.cpp client_irc.cpp ParsingChannelCommands.cpp ParsingUtils.cpp Channel.cpp

obj = $(src:.cpp=.o)

hdr = ft_irc.hpp client_irc.hpp ParsingChannelCommands.hpp Channel.hpp

all : $(NAME)

$(NAME) : $(obj)
	$(CC) $(FLAGS) $(obj) -o $(NAME)

%.o : %.cpp  $(hdr)
	$(CC) $(FLAGS) $< -c
clean :
	$(RM) $(obj)

fclean : clean
	$(RM) $(NAME)

re : fclean all