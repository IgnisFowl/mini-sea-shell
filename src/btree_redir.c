/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:30:07 by codespace         #+#    #+#             */
/*   Updated: 2025/06/27 12:10:46 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*append_redir(t_redir *head, t_redir *new)
{
	t_redir	*last;

	if (!head)
		return (new);
	last = head;
	while (last->next)
		last = last->next;
	last->next = new;
	return (head);
}

t_node	*attach_redirs(t_node *node, t_redir *redirs)
{
	t_node	*current;

	if (!node)
	{
		free_redirs(redirs);
		return (NULL);
	}
	if (!redirs)
		return (node);
	current = node;
	while (current && (current->type == T_PIPE || \
		current->type == T_AND || current->type == T_OR))
	{
		if (!current->left)
		{
			free_redirs(redirs);
			return (node);
		}
		current = current->left;
	}
	if (current && (current->type == T_WORD || \
		current->type == T_SQUOTE || current->type == T_DQUOTE || \
		current->type == T_GROUP))
		current->redir = append_redir(current->redir, redirs);
	return (node);
}

t_redir	*create_single_redir_node(t_token *current_token)
{
	t_redir	*new;

	new = (t_redir *)malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = current_token->type;
	new->filename = NULL;
	new->delimiter = NULL;
	if (new->type == T_HEREDOC)
		new->delimiter = ft_strdup(current_token->next->value);
	else
		new->filename = ft_strdup(current_token->next->value);
	if ((current_token->type == T_HEREDOC && !new->delimiter)
		|| (current_token->type != T_HEREDOC && !new->filename
			&& current_token->next->value != NULL))
	{
		free(new->delimiter);
		free(new->filename);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

t_redir	*parse_redirections(t_token **tokens)
{
	t_redir	*head;
	t_redir	*new;
	t_token	*current;

	head = NULL;
	if (!tokens || !*tokens)
		return (NULL);
	while (*tokens && ((*tokens)->type >= T_REDIR_IN && \
	(*tokens)->type <= T_HEREDOC))
	{
		current = *tokens;
		if (!current->next || !is_filename(current->next->type))
			break ;
		new = create_single_redir_node(current);
		if (!new)
		{
			free_redirs(head);
			return (NULL);
		}
		head = append_redir(head, new);
		*tokens = current->next->next;
	}
	return (head);
}
