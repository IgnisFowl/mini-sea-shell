/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:13:07 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/27 12:09:05 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_empty_path(t_exec *ctx, t_exec **ctx_out)
{
	int		err;

	if (ctx->err == EACCES || ctx->err == EISDIR)
	{
		ft_putendl_fd("minishell: Permission denied", 2);
		err = 126;
	}
	else if (ctx->err == ENOENT)
	{
		ft_putendl_fd("minishell: command not found", 2);
		err = 127;
	}
	else
	{
		ft_putendl_fd("minishell: unknown error", 2);
		err = 127;
	}
	free_all_execve(ctx);
	*ctx_out = NULL;
	return (err);
}

static int	prepare_exec_ctx(t_node *node, t_env *env, t_exec **ctx_out)
{
	t_exec	*ctx;

	if (!node || !node->cmd || node->cmd[0] == '\0')
		return (0);
	ctx = malloc(sizeof(t_exec));
	if (!ctx)
		return (1);
	init_ctx(ctx, node, env);
	if (!ctx->path || ctx->path[0] == '\0')
		return (handle_empty_path(ctx, ctx_out));
	*ctx_out = ctx;
	return (0);
}

static int	handle_exit_code(t_exec *ctx)
{
	int	exit_code;

	if (WIFEXITED(ctx->status))
		exit_code = WEXITSTATUS(ctx->status);
	else if (WIFSIGNALED(ctx->status))
	{
		exit_code = 128 + WTERMSIG(ctx->status);
		if (exit_code == 131)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (exit_code == 130)
			ft_putstr_fd("\n", 2);
	}
	else
		exit_code = EXIT_FAILURE;
	return (exit_code);
}

static int	handle_fork_and_wait(t_exec *ctx, t_env **env, t_node *root)
{
	int	exit_code;

	ctx->pid = fork();
	if (ctx->pid < 0)
	{
		perror("error in fork\n");
		free_all_execve(ctx);
		return (EXIT_FAILURE);
	}
	else if (ctx->pid == 0)
		execute_child_process(ctx, env, root);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(ctx->pid, &ctx->status, 0);
	handle_signals();
	exit_code = handle_exit_code(ctx);
	return (exit_code);
}

int	execute_command_node(t_node *node, t_env **env, t_node *root)
{
	t_exec	*ctx;
	int		err;
	int		exit_code;

	if (!node || !node->cmd || node->cmd[0] == '\0')
		return (EXIT_SUCCESS);
	err = prepare_exec_ctx(node, *env, &ctx);
	if (err != 0)
		return (err);
	exit_code = handle_fork_and_wait(ctx, env, root);
	free_all_execve(ctx);
	return (exit_code);
}
