/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 11:48:39 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/23 13:11:48 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_str(t_env **env, const char *key)
{
	char	*joined;
	char	*result;
	t_env	*tmp;

	tmp = *env;
	while (env)
	{
		if (ft_strcmp(tmp->key, key) == 0 && tmp->value)
		{
			joined = ft_strjoin(tmp->key, "=");
			if (!joined)
				return (NULL);
			result = ft_strjoin(joined, tmp->value);
			free(joined);
			return (result);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	set_env(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->next = *env;
	*env = tmp;
}

int	handle_builtin(t_node *root, t_env **env)
{
	if (!root || !root->cmd)
		return (0);
	if (ft_strcmp(root->cmd, "exit") == 0)
		return (exit_builtin(root, env));
	if (ft_strcmp(root->cmd, "cd") == 0)
		return (cd_builtin(root, env));
	if (ft_strcmp(root->cmd, "pwd") == 0)
		return (pwd_builtin());
	if (ft_strcmp(root->cmd, "env") == 0)
		return (env_builtin(*env));
	if (ft_strcmp(root->cmd, "export") == 0)
		return (export_builtin(root, env));
	if (ft_strcmp(root->cmd, "unset") == 0)
		return (unset_builtin(root, env));
	if (ft_strcmp(root->cmd, "echo") == 0)
		return (echo_builtin(root));
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
