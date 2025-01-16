#=====================================FLAGS=====================================#

CC = cc
FLAGS = -Wall -Wextra -Werror
LIBFT = -L$(LIBFT_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#===================================PATHS=======================================#

LIBFT_PATH = ./libft_plus/
HEADERS_PATH = ./include
SRCS_PATH = ./src
BONUS_PATH = ./src_bonus

#=================================FILES=========================================#

NAME = pipex
BONUS_NAME = pipex_bonus
SRC = $(wildcard $(SRCS_PATH)/*.c)
OBJ = $(SRC:.c=.o)
HEADER = $(HEADERS_PATH)/pipex.h
SRC_BONUS = $(wildcard $(BONUS_PATH)/*.c)
OBJ_BONUS = $(SRC_BONUS:.c=.o)
HEADER_BONUS = $(HEADERS_PATH)/pipex_bonus.h

#==================================RULES========================================#

all: libft $(NAME)

libft:
	$(MAKE_NO_PRINT) -C $(LIBFT_PATH)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

$(SRCS_PATH)/%.o: $(SRCS_PATH)/%.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@ -I $(HEADERS_PATH) -I $(LIBFT_PATH)

bonus: libft_plus $(BONUS_NAME)

$(BONUS_NAME): $(OBJ_BONUS) $(HEADER_BONUS)
	$(CC) $(FLAGS) -o $(BONUS_NAME) $(OBJ_BONUS) $(LIBFT)

$(BONUS_PATH)/%.o: $(BONUS_PATH)/%.c $(HEADER_BONUS)
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
	rm -f $(OBJ) $(OBJ_BONUS)
	$(MAKE_NO_PRINT) clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)
	$(MAKE_NO_PRINT) fclean -C $(LIBFT_PATH)

re: fclean all

rebonus: fclean libft bonus

.PHONY: all clean fclean re libft leak gdb run