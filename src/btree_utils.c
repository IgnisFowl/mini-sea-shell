/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:19 by codespace         #+#    #+#             */
/*   Updated: 2025/06/21 16:48:20 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*token_type_to_str(t_token_type type)
{
	char	*names[11];

	names[0] = "WORD";
	names[1] = "GROUP";
	names[2] = "SQUOTE";
	names[3] = "DQUOTE";
	names[4] = "PIPE";
	names[5] = "AND";
	names[6] = "OR";
	names[7] = "REDIR_IN";
	names[8] = "REDIR_OUT";
	names[9] = "REDIR_APPEND";
	names[10] = "HEREDOC";
	if (type < 0 || type > T_HEREDOC)
		return ("UNKOWN");
	return (names[type]);
}

int	is_token_empty(t_token *token)
{
	return (!token || !token->value || token->value[0] == '\0');
}

t_token	*find_last_operator(t_token *start, t_token_type type)
{
	t_token	*last;

	last = NULL;
	while (start)
	{
		if (!is_token_empty(start) && start->type == type)
			last = start;
		start = start->next;
	}
	return (last);
}

t_token	*find_token_before_op(t_token *start, t_token *target)
{
	t_token	*current;

	if (!start || !target || start == target)
		return (NULL);
	current = start;
	while (current->next)
	{
		if (current->next == target)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_node	*set_left_node(t_node *node, t_token *op, t_token **tokens)
{
	t_token	*token_before_op;
	t_token	*original;
	t_token	*left;

	token_before_op = NULL;
	original = NULL;
	left = *tokens;
	if (left != op)
	{
		token_before_op = find_token_before_op(left, op);
		if (token_before_op)
		{
			original = token_before_op->next;
			token_before_op->next = NULL;
		}
	}
	if (left == op)
		node->left = NULL;
	else
		node->left = parse_expression(&left);
	if (token_before_op)
		token_before_op->next = original;
	return (node);
}
