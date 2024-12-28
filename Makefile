#=====================================FLAGS=====================================#

CC = cc
FLAGS = -Wall -Wextra -Werror
LIBFT = -L$(LIBFT_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#===================================PATHS=======================================#

LIBFT_PATH = ./libft_plus/
HEADERS_PATH = ./include
SRCS_PATH = ./src

#=================================FILES=========================================#

NAME = pipex
SRC = $(wildcard $(SRCS_PATH)/*.c)
OBJ = $(SRC:.c=.o)
HEADER = $(HEADERS_PATH)/pipex.h

#==================================RULES========================================#

all: $(NAME)

libft:
	@$(MAKE_NO_PRINT) -C $(LIBFT_PATH)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

$(SRCS_PATH)/%.o: $(SRCS_PATH)/%.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@ -I $(HEADERS_PATH) -I $(LIBFT_PATH)

run:
	@echo "args: ./pipex file1 cmd1 cmd2 file2"
	./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)

#===================================LEAK=======================================#

leak:
	@echo "args: ./pipex file1 cmd1 cmd2 file2"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)

#===================================GDB=======================================#

gdb:
	@echo "args: ./pipex file1 cmd1 cmd2 file2"
	gdb --tui --args ./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)

#===================================CLEAN======================================#

clean:
	rm -f $(OBJ)
	@$(MAKE_NO_PRINT) clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME)
	@$(MAKE_NO_PRINT) fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re libft leak gdb run