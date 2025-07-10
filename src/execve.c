/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:00:15 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/28 13:33:07 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_group(t_node *node, t_env **env, t_node *root)
{
	int	pid;
	int	status;
	int	exit_code;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		status = execute_tree_recursive(node->left, env, root);
		free_tree(root);
		free_env(*env);
		rl_clear_history();
		exit(status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = 1;
	return (exit_code);
}

static int	handle_logic(t_node *node, t_env **env, t_node *root)
{
	int	status;

	if (node->type == T_AND)
	{
		status = execute_tree_recursive(node->left, env, root);
		if (status == 0)
			return (execute_tree_recursive(node->right, env, root));
		return (status);
	}
	else if (node->type == T_OR)
	{
		status = execute_tree_recursive(node->left, env, root);
		if (status != 0)
			return (execute_tree_recursive(node->right, env, root));
		return (status);
	}
	return (-1);
}

int	execute_tree_recursive(t_node *node, t_env **env, t_node *root)
{
	if (!node)
		return (0);
	if (node->type == T_GROUP)
		return (handle_group(node, env, root));
	else if (node->type == T_AND || node->type == T_OR)
		return (handle_logic(node, env, root));
	else if (node->type == T_PIPE)
		return (execute_pipe_node(node, env, root));
	else
		return (execute_command_node(node, env, root));
}

static int	execute_builtin(t_node *node, t_env **env)
{
	int	b_stdin;
	int	b_stdout;
	int	exit_status;

	if ((node->type == T_WORD || node->type == T_GROUP) \
		&& is_builtin(node->cmd))
	{
		b_stdin = dup(STDIN_FILENO);
		b_stdout = dup(STDOUT_FILENO);
		if (apply_redirs(node->redir) != 0)
		{
			dup2(b_stdin, STDIN_FILENO);
			dup2(b_stdout, STDOUT_FILENO);
			close(b_stdin);
			close(b_stdout);
			return (EXIT_FAILURE);
		}
		exit_status = handle_builtin(node, env);
		dup2(b_stdin, STDIN_FILENO);
		dup2(b_stdout, STDOUT_FILENO);
		close(b_stdin);
		close(b_stdout);
		return (exit_status);
	}
	return (execute_tree_recursive(node, env, node));
}

int	execute_tree(t_node *node, t_env **env)
{
	if (!node)
		return (0);
	if (process_heredocs_tree(node, env) != 0)
		return (g_last_signal);
	return (execute_builtin(node, env));
}
