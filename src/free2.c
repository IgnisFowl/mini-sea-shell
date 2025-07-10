/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:22:03 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 15:37:21 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_free(char *buffer, char *buf)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;

	if (!buffer && !buf)
		return (NULL);
	if (buffer)
		s1_len = ft_strlen(buffer);
	else
		s1_len = 0;
	if (buf)
		s2_len = ft_strlen(buf);
	else
		s2_len = 0;
	result = (char *)malloc(s1_len + s2_len + 1);
	if (!result)
		return (NULL);
	if (buffer)
		ft_memcpy(result, buffer, s1_len);
	if (buf)
		ft_memcpy(result + s1_len, buf, s2_len);
	result[s1_len + s2_len] = '\0';
	free(buffer);
	return (result);
}

void	free_env(t_env *env)
{
	t_env	*next;
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	env = NULL;
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_all_execve(t_exec *ctx)
{
	if (!ctx)
		return ;
	if (ctx->argv)
	{
		free_array(ctx->argv);
		ctx->argv = NULL;
	}
	if (ctx->envp)
	{
		free_array(ctx->envp);
		ctx->envp = NULL;
	}
	if (ctx->path)
	{
		free(ctx->path);
		ctx->path = NULL;
	}
	free(ctx);
}

void	free_pipe(t_env **env, t_node *root)
{
	if (root)
		free_tree(root);
	if (*env)
		free_env(*env);
	rl_clear_history();
}
