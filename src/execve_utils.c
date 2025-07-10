/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:00:01 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/26 23:17:35 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_arr(t_node *node, char **argv, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		argv[i + 1] = ft_strdup(node->args[i]);
		if (!argv[i + 1])
		{
			free_array(argv);
			return (0);
		}
		i++;
	}
	argv[len + 1] = NULL;
	return (1);
}

static char	*expand_special_vars(char *arg, int last_status)
{
	char	*res;

	if (ft_strcmp(arg, "$?") == 0)
		res = ft_itoa(last_status);
	else if (ft_strcmp(arg, "$$") == 0)
		res = ft_itoa(getpid());
	else
		return (ft_strdup(arg));
	return (res);
}

static void	handle_expansion(char **argv)
{
	int		i;
	char	*expanded;

	i = 0;
	while (argv[i])
	{
		expanded = expand_special_vars(argv[i], g_last_signal);
		free(argv[i]);
		argv[i] = expanded;
		i++;
	}
}

char	**build_argv_from_node(t_node	*node)
{
	char	**argv;
	int		arg_count;

	if (!node || !node->cmd)
		return (NULL);
	arg_count = argv_len(node);
	argv = malloc(sizeof(char *) * (arg_count + 2));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(node->cmd);
	if (!argv[0])
	{
		free_array(argv);
		return (NULL);
	}
	if (!build_arr(node, argv, arg_count))
		return (NULL);
	handle_expansion(argv);
	return (argv);
}

void	init_ctx(t_exec *ctx, t_node *node, t_env *env)
{
	ctx->argv = NULL;
	ctx->envp = NULL;
	ctx->path = NULL;
	ctx->status = 0;
	ctx->err = 0;
	ctx->pid = 0;
	ctx->node = node;
	ctx->env = env;
	if (node->cmd && node->cmd[0] != '\0')
	{
		ctx->argv = build_argv_from_node(ctx->node);
		ctx->envp = env_to_envp(ctx->env);
		if (!ctx->argv || !ctx->envp)
		{
			free_all_execve(ctx);
			return ;
		}
		ctx->path = get_full_path(ctx->node->cmd, ctx->envp, &ctx->err);
	}
	else
	{
		ctx->path = NULL;
		ctx->err = 0;
		return ;
	}
}
