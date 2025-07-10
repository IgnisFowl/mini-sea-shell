/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nade-lim <nade-lim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:13:57 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/26 17:44:15 by nade-lim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_key(const char *key)
{
	int	i;

	if (!key || !key[0] || ft_isdigit(key[0]) || key[0] == '=')
		return (0);
	i = 0;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	set_empty_value(char *arg, t_env **env)
{
	if (!is_valid_key(arg))
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		return (1);
	}
	set_env(env, arg, "");
	return (0);
}

static int	process_args(char *arg, t_env **env)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (!eq || eq == arg)
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		return (1);
	}
	key = ft_substr(arg, 0, eq - arg);
	if (!is_valid_key(key))
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		free(key);
		return (1);
	}
	value = ft_strdup(eq + 1);
	set_env(env, key, value);
	free(key);
	free(value);
	return (0);
}

int	export_builtin(t_node *node, t_env **env)
{
	int		i;
	char	*arg;
	int		exit_status;

	if (!node->args || !node->args[0])
	{
		print_export_sorted(*env);
		return (0);
	}
	i = 0;
	exit_status = 0;
	while (node->args[i])
	{
		arg = node->args[i];
		if (ft_strchr(arg, '='))
		{
			if (process_args(arg, env) != 0)
				exit_status = 1;
		}
		else
			if (set_empty_value(node->args[i], env) != 0)
				exit_status = 1;
		i++;
	}
	return (exit_status);
}
