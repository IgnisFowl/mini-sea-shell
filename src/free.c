/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:18:56 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/26 22:37:39 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->filename)
			free(redir->filename);
		if (redir->delimiter)
			free(redir->delimiter);
		free(redir);
		redir = temp;
	}
}

static void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_tree(t_node *node)
{
	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	free_redirs(node->redir);
	free_args(node->args);
	free(node->cmd);
	free(node);
	node = NULL;
}

void	free_tokens(t_token **tokens)
{
	t_token	*next;
	t_token	*current;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

void	free_all(t_node *node, char *line)
{
	if (node)
		free_tree(node);
	if (line)
		free(line);
}
