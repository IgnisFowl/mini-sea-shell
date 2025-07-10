/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:59:52 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 12:01:39 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_pipe_ctx(t_pipe *pipe_ctx, t_node *node, t_env **env)
{
	pipe_ctx->node = node;
	pipe_ctx->env = env;
	if (pipe(pipe_ctx->pfd) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	fork_left_child(t_pipe *pipe_ctx, t_node *root)
{
	pipe_ctx->pid_left = fork();
	if (pipe_ctx->pid_left < 0)
	{
		close(pipe_ctx->pfd[0]);
		close(pipe_ctx->pfd[1]);
		return (EXIT_FAILURE);
	}
	if (pipe_ctx->pid_left == 0)
	{
		close(pipe_ctx->pfd[0]);
		if (dup2(pipe_ctx->pfd[1], STDOUT_FILENO) == -1)
			exit (EXIT_FAILURE);
		close(pipe_ctx->pfd[1]);
		if (pipe_ctx->node->left->type == T_PIPE || \
			pipe_ctx->node->left->type == T_AND || \
		pipe_ctx->node->left->type == T_OR)
			handle_child_subtree(pipe_ctx->node->left, pipe_ctx->env, root);
		else
			handle_child_cmd(pipe_ctx->node->left, pipe_ctx->env, root);
	}
	return (EXIT_SUCCESS);
}

static int	fork_right_child(t_pipe *pipe_ctx, t_node *root)
{
	pipe_ctx->pid_right = fork();
	if (pipe_ctx->pid_right < 0)
	{
		close(pipe_ctx->pfd[0]);
		close(pipe_ctx->pfd[1]);
		kill(pipe_ctx->pid_left, SIGTERM);
		waitpid(pipe_ctx->pid_left, NULL, 0);
		return (EXIT_FAILURE);
	}
	if (pipe_ctx->pid_right == 0)
	{
		close(pipe_ctx->pfd[1]);
		if (dup2(pipe_ctx->pfd[0], STDIN_FILENO) == -1)
			exit (EXIT_FAILURE);
		close(pipe_ctx->pfd[0]);
		if (pipe_ctx->node->right->type == T_PIPE || \
			pipe_ctx->node->right->type == T_AND || \
		pipe_ctx->node->right->type == T_OR)
			handle_child_subtree(pipe_ctx->node->right, pipe_ctx->env, root);
		else
			handle_child_cmd(pipe_ctx->node->right, pipe_ctx->env, root);
	}
	return (EXIT_SUCCESS);
}

static int	parent_wait_pipe(t_pipe *pipe_ctx)
{
	int	exit_code;
	int	status_left;
	int	status_right;

	close(pipe_ctx->pfd[0]);
	close(pipe_ctx->pfd[1]);
	waitpid(pipe_ctx->pid_left, &status_left, 0);
	waitpid(pipe_ctx->pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
		exit_code = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
	{
		exit_code = 128 + WTERMSIG(status_right);
		if (exit_code == 131)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (exit_code == 130)
			ft_putstr_fd("\n", 2);
	}
	else
		exit_code = EXIT_FAILURE;
	return (exit_code);
}

int	execute_pipe_node(t_node *node, t_env **env, t_node *root)
{
	t_pipe	pipe_ctx;
	int		exit_code;

	exit_code = 0;
	if (!node->left || !node->right)
		return (EXIT_FAILURE);
	if (prepare_pipe_ctx(&pipe_ctx, node, env) != 0)
		return (EXIT_FAILURE);
	if (fork_left_child(&pipe_ctx, root) != 0)
		return (EXIT_FAILURE);
	if (fork_right_child(&pipe_ctx, root) != 0)
		return (EXIT_FAILURE);
	exit_code = parent_wait_pipe(&pipe_ctx);
	return (exit_code);
}
