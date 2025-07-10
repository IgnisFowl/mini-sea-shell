/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:07:30 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/23 13:16:58 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

static int	exit_shell(t_node *tree, t_env **env)
{
	int	exit_code;

	exit_code = g_last_signal;
	ft_putstr_fd("exit\n", 1);
	if (tree && tree->args && tree->args[1])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_last_signal = 1;
		return (1);
	}
	if (tree && tree->args && tree->args[0])
	{
		if (is_numeric(tree->args[0]))
			exit_code = ft_atoi(tree->args[0]) % 256;
		else
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit_code = 2;
		}
	}
	free_tree(tree);
	free_env(*env);
	rl_clear_history();
	exit(exit_code);
}

int	exit_builtin(t_node *root, t_env **env)
{
	return (exit_shell(root, env));
}
