/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:51:12 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/23 17:19:28 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(str);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, char *value, \
	t_token_type type, t_token_pos pos)
{
	t_token	*node;
	t_token	*tmp;

	node = new_token(value);
	if (!node)
		return ;
	node->type = type;
	node->pos = pos;
	if (!*head)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}
