/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_pipe_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:09:10 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/23 12:25:56 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_subtree(t_node *node, t_env **env, t_node *root)
{
	int	status;

	status = execute_tree_recursive(node, env, root);
	free_pipe(env, root);
	exit(status);
}

void	handle_child_cmd(t_node *node, t_env **env, t_node *root)
{
	t_exec	*ctx;

	ctx = malloc(sizeof(t_exec));
	if (!ctx)
	{
		free_pipe(env, root);
		exit(EXIT_FAILURE);
	}
	init_ctx(ctx, node, *env);
	if (!ctx->path || !ctx->path[0])
	{
		free_all_execve(ctx);
		free_pipe(env, root);
		exit(127);
	}
	execute_child_process(ctx, env, root);
}
