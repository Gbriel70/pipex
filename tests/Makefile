NAME = tester
BONUS_NAME = tester_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = main.c
BONUS = $(wildcard *bonus.c)
LIBFT = -L../libft_plus -lft

all: pipex $(NAME)
	./$(NAME)
	make --no-print fclean
	
pipex:
	$(MAKE) -C ../

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lrt -lm $(LIBFT)

bonus:
	$(MAKE) bonus -C ../
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS) -lrt -lm $(LIBFT)
	./$(BONUS_NAME)
	make --no-print fclean

leak: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
	make --no-print fclean

clean:
	rm -f *.o
	rm -f ../outfile*

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all pipex clean fclean re bonus