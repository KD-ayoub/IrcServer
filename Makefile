NAME = ircserv

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

RM = rm -rf

src = main.cpp ft_irc.cpp checkArguments.cpp outils.cpp client_irc.cpp 

obj = $(src:.cpp=.o)

hdr = ft_irc.hpp client_irc.hpp
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