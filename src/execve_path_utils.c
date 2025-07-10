/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_path_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:23:29 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/21 16:23:46 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	argv_len(t_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node->args && node->args[i])
		i++;
	return (i);
}

char	*join_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	if (!paths)
	{
		return (NULL);
	}
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_path_var(char **envp)
{
	int		i;
	char	*path_var;

	i = 0;
	path_var = NULL;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path_var);
}
