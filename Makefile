NAME = ircserv

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

RM = rm -rf

src = main.cpp ft_irc.cpp checkArguments.cpp ParsingChannelCommands.cpp ParsingUtils.cpp

obj = $(src:.cpp=.o)

all : $(NAME)

$(NAME) : $(obj)
	$(CC) $(FLAGS) $(obj) -o $(NAME)

%.o : %.cpp  ft_irc.hpp ParsingChannelCommands.hpp
	$(CC) $(FLAGS) $< -c
clean :
	$(RM) $(obj)

fclean : clean
	$(RM) $(NAME)

re : fclean all