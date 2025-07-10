# mini C shell

valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell

