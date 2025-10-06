SRCS = main.c \
	utils.c

OBJS = $(SRCS:.c=.o)

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0 #-fsanitize=thread #-fsanitize=address
LINKS = -lpthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LINKS) $(OBJS) -o $(NAME)
	dsymutil $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
