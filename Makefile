NAME	=	ircserv

SRC		=	main.cpp \
			Clients/Clients.cpp \
			Chat/Chat.cpp \
			Chat/CommandChat.cpp \
			Channel/Channel.cpp \
			Utils/utils.cpp \

CC		=	clang++

FLAGS	=	-Wall -Wextra -Werror

OBJS	=	${SRC:.cpp=.o}

$(NAME): $(OBJS)
		$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re:	fclean all

run: fclean all
	make clean && clear && ./$(NAME)

.PHONY: all clean fclean re