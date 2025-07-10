/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nade-lim <nade-lim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:25:29 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/26 17:41:29 by nade-lim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_variable(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	unset_builtin(t_node *node, t_env **env)
{
	int	i;

	if (!node || !node->args)
		return (0);
	i = 0;
	while (node->args[i])
	{
		unset_variable(env, node->args[i]);
		i++;
	}
	return (0);
}
