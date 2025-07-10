/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_parens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:14:21 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:15:49 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_parens(char c, int *parens, int squote, int dquote)
{
	if (c == '(' && !squote && !dquote)
		(*parens)++;
	else if (c == ')' && !squote && !dquote)
	{
		if ((*parens) == 0)
		{
			ft_putstr_fd("Syntax error: unmatched ')'\n", 2);
			return (0);
		}
		(*parens)--;
	}
	return (1);
}

int	check_parenthesis(const char *str)
{
	int	i;
	int	parens;
	int	squote;
	int	dquote;

	i = 0;
	parens = 0;
	squote = 0;
	dquote = 0;
	while (str[i])
	{
		check_quotes_state(str[i], &squote, &dquote);
		if (!handle_parens(str[i], &parens, squote, dquote))
			return (0);
		i++;
	}
	if (parens != 0)
	{
		ft_putstr_fd("Syntax error: unmatched ')'\n", 2);
		return (0);
	}
	return (1);
}

int	check_brackets(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '{')
		{
			i++;
			if (line[i] == '}')
			{
				ft_putstr_fd("Syntax error: empty variable name in {}\n", 2);
				return (0);
			}
			while (line[i] && line[i] != '}')
				i++;
			if (line[i] != '}')
			{
				ft_putstr_fd("Syntax error: unmatched bracket\n", 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}
