NAME = rush-02

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = \
	rush-02.c \
	input_check.c \
	number_builder.c \
	output.c

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re