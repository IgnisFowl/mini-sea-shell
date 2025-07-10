/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:15:50 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/18 13:39:36 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*free_partial_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
	return (NULL);
}

static void	assign_env_values(char *equal, t_env *node, char **envp, int i)
{
	if (equal)
	{
		node->key = ft_substr(envp[i], 0, equal - envp[i]);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(envp[i]);
		node->value = NULL;
	}
}

t_env	*env_from_envp(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*equal;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (free_partial_env(head));
		equal = ft_strchr(envp[i], '=');
		assign_env_values(equal, node, envp, i);
		node->next = head;
		head = node;
		i++;
	}
	return (head);
}

static int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_envp(t_env *env)
{
	int		i;
	int		size;
	char	**envp;
	char	*joined;

	i = 0;
	size = env_list_size(env);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	while (env)
	{
		if (env->value)
		{
			joined = ft_strjoin(env->key, "=");
			envp[i++] = ft_strjoin(joined, env->value);
			free(joined);
		}
		else
			envp[i++] = ft_strdup(env->key);
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
