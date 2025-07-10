/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:25:06 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/13 15:38:08 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_filename(t_token_type type)
{
	return (type == T_WORD || type == T_SQUOTE || type == T_DQUOTE);
}

t_redir	*init_redir_node(t_token_type type, const char *value)
{
	t_redir	*new_node;

	new_node = (t_redir *)malloc(sizeof(t_redir));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->filename = ft_strdup(value);
	if (!new_node->filename && value != NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_node	*init_op_node_base(t_token_type type, const char *value)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	if (value)
		node->cmd = ft_strdup(value);
	else
		node->cmd = NULL;
	if (value && !node->cmd)
	{
		free(node);
		return (NULL);
	}
	node->args = NULL;
	node->redir = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node	*create_redir_holder_node(t_redir *redirs)
{
	t_node	*holder;

	if (!redirs)
		return (NULL);
	holder = (t_node *)malloc(sizeof(t_node));
	if (!holder)
	{
		free_redirs(redirs);
		return (NULL);
	}
	holder->type = 0;
	holder->cmd = NULL;
	holder->args = NULL;
	holder->left = NULL;
	holder->right = NULL;
	holder->redir = redirs;
	return (holder);
}

t_node	*assign_child_with_redirs(t_node *child, t_redir *redirs)
{
	if (child)
		return (attach_redirs(child, redirs));
	else if (redirs)
		return (create_redir_holder_node(redirs));
	return (NULL);
}
