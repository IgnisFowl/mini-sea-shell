/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:22:38 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/26 14:16:07 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redirs_or_exit(t_exec *ctx, t_env **env, t_node *root)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirs(ctx->node->redir) != 0)
	{
		free_all_execve(ctx);
		free_pipe(env, root);
		exit(EXIT_FAILURE);
	}
}

static void	handle_builtin_or_exit(t_exec *ctx, t_env **env, t_node *root)
{
	int	builtin_status;

	if (is_builtin(ctx->node->cmd))
	{
		builtin_status = handle_builtin(ctx->node, env);
		free_all_execve(ctx);
		free_pipe(env, root);
		exit(builtin_status);
	}
}

static void	handle_exec_or_exit(t_exec *ctx, t_env **env, t_node *root)
{
	if (!ctx->path || !ctx->argv || !ctx->argv[0])
	{
		free_all_execve(ctx);
		free_pipe(env, root);
		if (ctx->err == EISDIR)
			exit(126);
		else if (ctx->err == EACCES)
			exit(126);
		else if (ctx->err == ENOENT)
			exit(127);
		else
			exit(127);
	}
	execve(ctx->path, ctx->argv, ctx->envp);
	perror(ctx->argv[0]);
	free_all_execve(ctx);
	free_pipe(env, root);
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(127);
}

void	execute_child_process(t_exec *ctx, t_env **env, t_node *root)
{
	handle_redirs_or_exit(ctx, env, root);
	handle_builtin_or_exit(ctx, env, root);
	handle_exec_or_exit(ctx, env, root);
}
