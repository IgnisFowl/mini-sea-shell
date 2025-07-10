/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 10:17:16 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:18:31 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_adjacent_tokens(t_token *head)
{
	t_token	*curr;
	t_token	*next;
	char	*joined;

	curr = head;
	while (curr && curr->next)
	{
		next = curr->next;
		if ((is_word(curr)) && (is_word(next)) \
		&& curr->pos.end == next->pos.start)
		{
			joined = ft_strjoin(curr->value, next->value);
			if (!joined)
				return ;
			free(curr->value);
			curr->value = joined;
			curr->pos.end = next->pos.end;
			curr->next = next->next;
			free(next->value);
			free(next);
		}
		else
			curr = curr->next;
	}
}
