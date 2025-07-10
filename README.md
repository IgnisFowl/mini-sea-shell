# mini C shell

By me and @nathalialmota.

The Minishell project is part of the 42 curriculum and challenges students to build a simplified version of the Unix shell from scratch in C. Its goal is to deepen understanding of processes, file descriptors, environment variables, signals, parsing, and memory management. 

The shell must interpret and execute commands typed by the user, handle built-ins, redirections, pipelines, heredocs, and properly manage signals like Ctrl+C and Ctrl+\. 

One of the main challenges lies in parsing and executing complex command lines while maintaining correct operator precedence (like &&, ||, and |) and supporting grouping via parentheses. Managing memory without leaks, especially when errors or signals interrupt execution (e.g., during heredoc input), also adds significant complexity. 

Recreating Bash-like behavior in all edge cases while keeping code clean, modular, and robust has been a demanding but rewarding experience.

To run it with the suppressor for readline leaks, use the following command:

valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell


