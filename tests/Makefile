NAME = tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = main.c
LIBFT = -L../libft_plus -lft

all: pipex $(NAME)
	./$(NAME)
	make --no-print fclean
	
pipex:
	$(MAKE) -C ../

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lrt -lm $(LIBFT)

clean:
	rm -f *.o
	rm -f ../outfile*

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all pipex clean fclean re