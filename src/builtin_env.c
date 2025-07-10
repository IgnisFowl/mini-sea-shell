/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:53:24 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 13:06:29 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

static int	check_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo_builtin(t_node *node)
{
	int	i;
	int	no_newline;

	i = 0;
	no_newline = 0;
	while (node->args && node->args[i] && check_n_option(node->args[i]))
	{
		no_newline = 1;
		i++;
	}
	while (node->args && node->args[i])
	{
		printf("%s", node->args[i]);
		if (node->args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}

void	update_pwd_vars(t_env **env, const char *oldpwd)
{
	char	cwd[1024];
	char	*new_pwd;

	if (!getcwd(cwd, sizeof(cwd)))
		return ;
	new_pwd = ft_strdup(cwd);
	if (!new_pwd)
		return ;
	set_env(env, "OLDPWD", oldpwd);
	set_env(env, "PWD", new_pwd);
	free(new_pwd);
}

int	pwd_builtin(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
