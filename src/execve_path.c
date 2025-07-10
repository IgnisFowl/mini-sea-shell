/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:57:57 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/23 17:14:28 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*validade_path_candidate(char *cmd, int *err)
{
	struct stat	statbuf;

	if (stat(cmd, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			*err = EISDIR;
			return (NULL);
		}
		if (access(cmd, X_OK) != 0)
		{
			*err = errno;
			return (NULL);
		}
		return (ft_strdup(cmd));
	}
	*err = errno;
	return (NULL);
}

static char	*join_dir_cmd(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

static char	*search_in_path(char **paths, char *cmd, int *err)
{
	char	*candidate;
	char	*result;
	int		i;

	i = 0;
	while (paths[i])
	{
		candidate = join_dir_cmd(paths[i], cmd);
		if (!candidate)
		{
			*err = ENOMEM;
			break ;
		}
		result = validade_path_candidate(candidate, err);
		free(candidate);
		if (result)
			return (result);
		i++;
	}
	if (*err == 0)
		*err = ENOENT;
	return (NULL);
}

static char	*get_path_from_env(char *cmd, char **envp, int *err)
{
	char		*path_var;
	char		**paths;
	char		*result;

	path_var = get_path_var(envp);
	if (!path_var)
	{
		*err = ENOENT;
		return (NULL);
	}
	paths = ft_split(path_var, ':');
	if (!paths)
	{
		*err = ENOMEM;
		return (NULL);
	}
	result = search_in_path(paths, cmd, err);
	free_array(paths);
	return (result);
}

char	*get_full_path(char *cmd, char **envp, int *err)
{
	if (!cmd || !cmd[0])
	{
		*err = ENOENT;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
		return (validade_path_candidate(cmd, err));
	return (get_path_from_env(cmd, envp, err));
}
