/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:03:34 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 13:08:58 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_dp_value(t_env **env, const char *key)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*handle_squiggle(t_node *node, t_env **env)
{
	const char	*home_path;
	char		*expanded;
	size_t		home_len;
	size_t		arg_len;

	home_path = get_env_value(*env, "HOME");
	if (!home_path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (NULL);
	}
	home_len = ft_strlen(home_path);
	arg_len = ft_strlen(node->args[0]);
	expanded = malloc(home_len + arg_len);
	if (!expanded)
		return (NULL);
	ft_strlcpy(expanded, home_path, home_len + 1);
	ft_strlcpy(expanded + home_len, node->args[0] + 1, arg_len);
	return (expanded);
}

static const char	*get_cd_target_path(t_node *node, t_env **env)
{
	const char	*home_path;

	if (!node->args || !node->args[0])
	{
		home_path = get_env_dp_value(env, "HOME");
		if (!home_path)
			ft_putstr_fd("cd: HOME not set\n", 2);
		return (home_path);
	}
	else if (node->args[1])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (NULL);
	}
	if (node->args[0][0] == '~')
		return (handle_squiggle(node, env));
	return (node->args[0]);
}

static int	change_dir(t_node *node, t_env **env, \
	const char *path, char *oldpwd)
{
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		if (node->args[0] && node->args[0][0] == '~')
			free((char *)path);
		return (1);
	}
	update_pwd_vars(env, oldpwd);
	free(oldpwd);
	if (node && node->args && node->args[0] && node->args[0][0] == '~')
		free((char *)path);
	return (0);
}

int	cd_builtin(t_node *node, t_env **env)
{
	const char	*path;
	char		*oldpwd;
	char		*pwd_val;

	if (node->args && node->args[1])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	pwd_val = get_env_dp_value(env, "PWD");
	if (pwd_val)
		oldpwd = ft_strdup(pwd_val);
	else
		oldpwd = ft_strdup("");
	path = get_cd_target_path(node, env);
	return (change_dir(node, env, path, oldpwd));
}
