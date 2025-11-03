SRCS = main.c \
	routines.c \
	solo_philo.c \
	monitoring.c \
	mutexes.c \
	utils.c

OBJS = $(SRCS:.c=.o)

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror #-g #-fsanitize=address #-fsanitize=thread
LINKS = -lpthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LINKS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
