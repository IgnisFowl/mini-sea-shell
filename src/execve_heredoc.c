/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 23:20:42 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 15:53:06 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_heredoc_vars(t_heredoc_vars *vars)
{
	vars->tmp_filename = NULL;
	vars->fd = -1;
	vars->pid = -1;
	vars->status = 0;
	vars->tmp_filename = generate_tmp_filename();
	if (!vars->tmp_filename)
		return (1);
	vars->fd = open(vars->tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (vars->fd < 0)
	{
		free(vars->tmp_filename);
		return (1);
	}
	return (0);
}

static void	run_heredoc_child_process(t_heredoc_vars *vars, \
	t_redir *redir, t_env *env, t_node *root)
{
	int	read_status;

	signal(SIGINT, heredoc_child_sigint);
	read_status = read_heredoc_input(vars->fd, redir, env);
	close(vars->fd);
	free(vars->tmp_filename);
	free_tree(root);
	free_env(env);
	rl_clear_history();
	exit(read_status);
}

static int	wait_for_heredoc_child(t_heredoc_vars *vars, t_redir *redir, \
	void (*original_sigint)(int), void (*original_sigquit)(int))
{
	struct termios	original_termios;
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &original_termios);
	new_termios = original_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	close(vars->fd);
	waitpid(vars->pid, &vars->status, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
	signal(SIGINT, original_sigint);
	signal(SIGQUIT, original_sigquit);
	if (WIFEXITED(vars->status) && WEXITSTATUS(vars->status) != 0)
	{
		if (WEXITSTATUS(vars->status) == 130)
			g_last_signal = 130;
		unlink(vars->tmp_filename);
		free(vars->tmp_filename);
		return (1);
	}
	free(redir->delimiter);
	redir->delimiter = NULL;
	redir->filename = vars->tmp_filename;
	redir->type = T_REDIR_IN;
	return (0);
}

static int	handle_single_heredoc(t_redir *redir, t_env *env, t_node *root)
{
	t_heredoc_vars	vars;
	void			(*original_sigint)(int);
	void			(*original_sigquit)(int);

	if (init_heredoc_vars(&vars) != 0)
		return (1);
	original_sigint = signal(SIGINT, SIG_IGN);
	original_sigquit = signal(SIGQUIT, SIG_IGN);
	vars.pid = fork();
	if (vars.pid < 0)
	{
		signal(SIGINT, original_sigint);
		signal(SIGQUIT, original_sigquit);
		close(vars.fd);
		unlink(vars.tmp_filename);
		free(vars.tmp_filename);
		return (1);
	}
	if (vars.pid == 0)
		run_heredoc_child_process(&vars, redir, env, root);
	return (wait_for_heredoc_child(&vars, \
		redir, original_sigint, original_sigquit));
}

int	process_heredocs_tree(t_node *node, t_env **env)
{
	t_redir	*current;

	if (!node)
		return (0);
	current = node->redir;
	while (current)
	{
		if (current->type == T_HEREDOC)
		{
			if (handle_single_heredoc(current, *env, node) != 0)
				return (1);
		}
		current = current->next;
	}
	if (process_heredocs_tree(node->left, env) != 0)
		return (1);
	if (process_heredocs_tree(node->right, env) != 0)
		return (1);
	return (0);
}
