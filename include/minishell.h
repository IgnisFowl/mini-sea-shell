/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:49:52 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/28 15:58:55 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_last_signal;

typedef enum e_token_type
{
	T_WORD,
	T_GROUP,
	T_SQUOTE,
	T_DQUOTE,
	T_PIPE,
	T_AND,
	T_OR,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}	t_token_type;

typedef struct s_heredoc_vars
{
	pid_t	pid;
	int		status;
	char	*tmp_filename;
	int		fd;
}	t_heredoc_vars;

typedef struct s_token_pos
{
	int	start;
	int	end;
}	t_token_pos;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	char			*delimiter;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	t_token_pos		pos;
}	t_token;

typedef struct s_node
{
	t_token_type	type;
	char			*cmd;
	char			**args;
	struct s_redir	*redir;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_exec
{
	char	**argv;
	char	**envp;
	char	*path;
	int		status;
	int		err;
	pid_t	pid;
	t_node	*node;
	t_env	*env;
}	t_exec;

typedef struct s_pipe
{
	t_node	*node;
	t_env	**env;
	int		pfd[2];
	pid_t	pid_left;
	pid_t	pid_right;
}	t_pipe;

void	free_all(t_node *node, char *line);
void	free_tree(t_node *node);
void	free_tokens(t_token **tokens);
void	free_redirs(t_redir *redir);
void	free_array(char **arr);
void	free_all_execve(t_exec *ctx);
void	free_env(t_env *env);
void	free_pipe(t_env **env, t_node *root);

int		checker(char *line);
int		check_quotes(char *line);
int		check_semicolon(const char *line);
int		check_redirections(const char *line);
int		check_pipes(const char *line);
int		check_brackets(char *line);
int		check_escapes(const char *line);
void	check_quotes_state(char c, int *squote, int *dquote);
int		check_parenthesis(const char *str);
int		is_space(char c);
int		ft_strcmp(const char *s1, const char *s2);
int		is_valid_varchar(char c, int first);
int		handle_variable(const char *str, int *i, int dquote);

void	tokenize(t_token **head, char *line);
t_token	*new_token(char *str);
void	add_token(t_token **head, char *value, \
		t_token_type type, t_token_pos pos);
void	tokenize_spaces(t_token **head, int *i, char *line);
char	*tokenize_word(char *line, int start, int len);
void	tokenize_simple_quote(t_token **head, int *i, char *line);
void	tokenize_double_quote(t_token **head, int *i, char *line);
void	tokenize_out(t_token **head, int *i, char *line);
void	tokenize_in(t_token **head, int *i, char *line);
int		is_metachar(char c);

t_node	*parse_expression(t_token **tokens);
t_redir	*parse_redirections(t_token **tokens);
t_node	*parse_cmd(t_token **tokens);
t_node	*parse_group(t_token **tokens);
char	*token_type_to_str(t_token_type type);
t_token	*find_last_operator(t_token *start, t_token_type type);
t_token	*find_token_before_op(t_token *start, t_token *target);
t_node	*create_op_node(t_token *op, t_token **tokens, t_redir *pre_redir);
t_redir	*append_redir(t_redir *head, t_redir *new);
t_node	*attach_redirs(t_node *node, t_redir *redirs);
t_node	*init_node(t_token **tokens);
t_node	*set_left_node(t_node *node, t_token *op, t_token **tokens);
void	free_partial_op_node(t_node *op, t_node *left, t_node *right);
int		is_filename(t_token_type type);
t_redir	*init_redir_node(t_token_type type, const char *value);
t_node	*init_op_node_base(t_token_type type, const char *value);
t_node	*create_redir_holder_node(t_redir *redirs);
t_node	*assign_child_with_redirs(t_node *child, t_redir *redirs);
t_node	*left_child_and_redirs(t_node *node, t_token *op, \
		t_token **tokens, t_redir *redirs);
t_node	*right_child_and_redirs(t_node *node, t_token *op, t_token **tokens);

t_env	*env_from_envp(char **envp);
char	**env_to_envp(t_env *env);

int		execute_tree(t_node *node, t_env **env);
int		execute_tree_recursive(t_node *node, t_env **env, t_node *root);
void	execute_child_process(t_exec *ctx, t_env **env, t_node *root);
int		execute_command_node(t_node *node, t_env **env, t_node *root);
void	init_ctx(t_exec *ctx, t_node *node, t_env *env);
char	**build_argv_from_node(t_node	*node);
int		argv_len(t_node *node);
char	*get_full_path(char *cmd, char **envp, int *err);
int		apply_redirs(t_redir *redir_list);
int		prepare_pipe_ctx(t_pipe *pipe_ctx, t_node *node, t_env **env);
int		execute_pipe_node(t_node *node, t_env **env, t_node *root);
void	handle_child_cmd(t_node *node, t_env **env, t_node *root);
void	handle_child_subtree(t_node *node, t_env **env, t_node *root);
int		process_heredocs_tree(t_node *node, t_env **env);

void	handle_sigint(int sig);
void	handle_signals(void);
void	run_shell_loop(t_env **env);
int		checker(char *line);

char	*ft_strjoin_free(char *s1, char *s2);
char	*get_env_value(t_env *env, const char *key);
int		get_var_length(const char *str);
int		get_braced_var_length(const char *str, int *i);
char	*expand_variables(const char *str, t_env *env);
char	*append_char(char *res, char c);
char	*expand_and_append(char *res, const char *str, t_env *env, int *i);
char	*expand_variables(const char *str, t_env *env);

int		handle_builtin(t_node *root, t_env **env);
char	*get_env_str(t_env **env, const char *key);
void	set_env(t_env **env, const char *key, const char *value);
int		cd_builtin(t_node *node, t_env **env);
int		pwd_builtin(void);
int		env_builtin(t_env *env);
int		export_builtin(t_node *node, t_env **env);
void	print_export_sorted(t_env *env);
int		unset_builtin(t_node *node, t_env **env);
int		echo_builtin(t_node *node);
int		is_builtin(char *cmd);
void	update_pwd_vars(t_env **env, const char *oldpwd);

void	merge_adjacent_tokens(t_token *head);
int		argv_len(t_node *node);
char	*join_path(char **paths, char *cmd);
char	*get_path_var(char **envp);
int		is_token_empty(t_token *token);
int		exit_builtin(t_node *root, t_env **env);
int		is_word(t_token *curr);

char	*get_next_line(int fd);
char	*ft_free(char *buffer, char *buf);
void	heredoc_child_sigint(int sig);
char	*generate_tmp_filename(void);
int		process_heredoc_line(char *line, int fd, t_redir *redir, t_env *env);
int		handle_heredoc_read(char **line);
int		read_heredoc_input(int fd, t_redir *redir, t_env *env);

#endif