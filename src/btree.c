/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:57:34 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/26 22:41:28 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_redir_first(t_node *node, \
	t_redir *head_redir, t_token **tokens)
{
	t_redir	*tmp;

	tmp = NULL;
	if (node)
	{
		node = attach_redirs(node, head_redir);
		if (node->type == T_GROUP)
		{
			tmp = parse_redirections(tokens);
			node = attach_redirs(node, tmp);
		}
	}
	else if (head_redir)
	{
		node = malloc(sizeof(t_node));
		if (!node)
			return (NULL);
		node->cmd = NULL;
		node->args = NULL;
		node->type = 0;
		node->left = NULL;
		node->right = NULL;
		node->redir = head_redir;
	}
	return (node);
}

t_node	*create_op_node(t_token *op, t_token **tokens, t_redir *pre_redir)
{
	t_node	*node;

	node = init_op_node_base(op->type, token_type_to_str(op->type));
	if (!node)
	{
		free_redirs(pre_redir);
		return (NULL);
	}
	if (left_child_and_redirs(node, op, tokens, pre_redir) == NULL)
	{
		free_redirs(pre_redir);
		free_tree(node);
		return (NULL);
	}
	if (right_child_and_redirs(node, op, tokens) == NULL)
	{
		free_tree(node);
		return (NULL);
	}
	return (node);
}

t_node	*parse_expression(t_token **tokens)
{
	t_token	*op;
	t_node	*node;
	t_redir	*head_redir;

	while (*tokens && is_token_empty(*tokens))
		*tokens = (*tokens)->next;
	if (!tokens || !*tokens)
		return (NULL);
	head_redir = parse_redirections(tokens);
	op = find_last_operator(*tokens, T_OR);
	if (op)
		return (create_op_node(op, tokens, head_redir));
	op = find_last_operator(*tokens, T_AND);
	if (op)
		return (create_op_node(op, tokens, head_redir));
	op = find_last_operator(*tokens, T_PIPE);
	if (op)
		return (create_op_node(op, tokens, head_redir));
	node = parse_cmd(tokens);
	if (head_redir)
		node = parse_redir_first(node, head_redir, tokens);
	return (node);
}
