/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_node2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:50:51 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/13 15:38:10 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_partial_op_node(t_node *op, t_node *left, t_node *right)
{
	if (op)
	{
		if (op->cmd)
			free(op->cmd);
		if (op->left != left)
			free_tree(op->left);
		else
			free_tree(left);
		if (op->right != right)
			free_tree(op->right);
		else
			free_tree(right);
	}
	else
	{
		free_tree(left);
		free_tree(right);
	}
}

t_node	*left_child_and_redirs(t_node *node, t_token *op, \
	t_token **tokens, t_redir *redirs)
{
	t_node	*original_parsed_left;

	node = set_left_node(node, op, tokens);
	if (!node)
	{
		free_redirs(redirs);
		return (NULL);
	}
	original_parsed_left = node->left;
	node->left = assign_child_with_redirs(original_parsed_left, redirs);
	if (redirs && !node->left)
	{
		free_partial_op_node(node, original_parsed_left, NULL);
		return (NULL);
	}
	return (node);
}

t_node	*right_child_and_redirs(t_node *node, t_token *op, t_token **tokens)
{
	t_redir	*redirs;
	t_node	*original_parsed_right;

	*tokens = op->next;
	redirs = parse_redirections(tokens);
	if (!redirs && *tokens && ((*tokens)->type >= \
	T_REDIR_IN && (*tokens)->type == T_HEREDOC))
	{
		free_partial_op_node(node, node->left, NULL);
		return (NULL);
	}
	original_parsed_right = parse_expression(tokens);
	node->right = assign_child_with_redirs(original_parsed_right, redirs);
	if (redirs && !node->right)
	{
		free_partial_op_node(node, node->left, original_parsed_right);
		return (NULL);
	}
	return (node);
}
