/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:45:26 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/13 15:38:06 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_group_node(t_node *node)
{
	node->type = T_GROUP;
	node->cmd = NULL;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->redir = NULL;
}

static t_node	*parse_internal_group_expression(char *group_value)
{
	t_token	*sub_tokens;
	t_token	*current_sub_token;
	t_node	*left;

	if (!group_value)
		return (NULL);
	sub_tokens = NULL;
	tokenize(&sub_tokens, group_value);
	left = NULL;
	current_sub_token = sub_tokens;
	if (current_sub_token)
		left = parse_expression(&current_sub_token);
	if (sub_tokens)
		free_tokens(&sub_tokens);
	return (left);
}

t_node	*parse_group(t_token **tokens)
{
	t_node	*group_node;
	t_token	*current;

	if (!tokens || !*tokens || (*tokens)->type != T_GROUP)
		return (NULL);
	current = *tokens;
	group_node = malloc(sizeof(t_node));
	if (!group_node)
		return (NULL);
	init_group_node(group_node);
	group_node->left = parse_internal_group_expression(current->value);
	*tokens = current->next;
	group_node->redir = parse_redirections(tokens);
	return (group_node);
}
