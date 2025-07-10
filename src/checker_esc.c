/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_esc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:10:28 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:02:06 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quotes_state(char c, int *squote, int *dquote)
{
	if (c == '\'' && !(*dquote))
		*squote = !(*squote);
	else if (c == '"' && !(*squote))
		*dquote = !(*dquote);
}

static int	is_escaped_char(char c)
{
	return (c == '"' || c == '\\' || c == '$' || c == '`');
}

int	check_escapes(const char *line)
{
	int	i;
	int	squote;
	int	dquote;

	i = 0;
	squote = 0;
	dquote = 0;
	while (line[i])
	{
		check_quotes_state(line[i], &squote, &dquote);
		if (line[i] == '\\' && !squote)
		{
			if (!line[i + 1])
				return (fprintf(stderr, \
					"Syntax error: error in '\\' use\n"), 0);
			if (dquote && !is_escaped_char(line[i + 1]))
				;
			else
				i++;
		}
		i++;
	}
	if (squote || dquote)
		return (fprintf(stderr, "Syntax error: unmatched quote\n"), 0);
	return (1);
}

int	check_semicolon(const char *line)
{
	int	i;
	int	squote;
	int	dquote;

	i = 0;
	squote = 0;
	dquote = 0;
	while (line[i])
	{
		check_quotes_state(line[i], &squote, &dquote);
		if (line[i] == ';' && !squote && !dquote)
		{
			ft_putstr_fd("Syntax error: unexpected token ';'\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}
