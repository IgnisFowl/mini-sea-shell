NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror # -g -fsanitize=address
LDFLAGS = -lreadline -lhistory

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDE = -I./include

# Source files inside src/
SRC = main.c utils.c checker.c checker_esc.c checker_var.c checker_redir.c \
	token.c token_quotes.c token_utils.c token_words.c free.c free2.c \
	btree.c btree_redir.c btree_utils.c btree_node.c btree_node2.c \
	btree_cmd.c btree_group.c envp.c execve.c execve_utils.c \
	execve_path.c execve_redir.c execve_pipe.c execve_pipe_utils.c \
	execve_heredoc.c signals.c shell_loop.c var_exp.c var_exp_utils.c \
	builtin.c builtin_cd.c builtin_exit.c builtin_env.c builtin_export.c \
	builtin_export_utils.c builtin_unset.c token_merge.c checker_parens.c \
	execve_path_utils.c execve_cmd.c execve_child.c get_next_line.c execve_heredoc_utils.c

SRC := $(addprefix $(SRC_DIR)/, $(SRC))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
