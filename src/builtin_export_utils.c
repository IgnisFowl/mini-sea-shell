/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:58:26 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/27 12:10:24 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_format(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

static t_env	*copy_env_list(t_env *env)
{
	t_env	*copy;
	t_env	*new;

	copy = NULL;
	while (env)
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		new->key = ft_strdup(env->key);
		if (env->value)
			new->value = ft_strdup(env->value);
		else
			new->value = NULL;
		new->next = copy;
		copy = new;
		env = env->next;
	}
	return (copy);
}

static void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = a->key;
	tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

static void	sort_env(t_env *env)
{
	t_env	*cur;
	t_env	*next;

	while (env)
	{
		cur = env;
		next = env->next;
		while (next)
		{
			if (ft_strcmp(cur->key, next->key) > 0)
				swap_env_nodes(cur, next);
			next = next->next;
		}
		env = env->next;
	}
}

void	print_export_sorted(t_env *env)
{
	t_env	*copy;

	copy = copy_env_list(env);
	if (!copy)
		return ;
	sort_env(copy);
	print_export_format(copy);
	free_env(copy);
}
