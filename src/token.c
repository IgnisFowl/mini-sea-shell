/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:34:03 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/18 17:43:34 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metachar(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>' || c == '(' || c == ')' || \
		c == '\'' || c == '"')
		return (1);
	return (0);
}

static void	tokenize_pipe(t_token **head, int *i, char *line)
{
	t_token_pos	pos;

	pos.start = *i;
	if (line[*i] == '|')
	{
		if (line[*i + 1] == '|')
		{
			pos.end = *i;
			add_token(head, "||", T_OR, pos);
			(*i)++;
		}
		else
		{
			pos.end = *i;
			add_token(head, "|", T_PIPE, pos);
		}
	}
	if (line[*i] == '&' && line[*i + 1] == '&')
	{
		pos.end = *i;
		add_token(head, "&&", T_AND, pos);
		(*i)++;
	}
	(*i)++;
}

static void	tokenize_parenthesis(t_token **head, int *i, char *line)
{
	t_token_pos	pos;
	int			depth;
	char		*tmp;

	depth = 1;
	(*i)++;
	pos.start = *i;
	while (line[*i] && depth > 0)
	{
		if (line[*i] == '(')
			depth++;
		else if (line[*i] == ')')
			depth--;
		(*i)++;
	}
	if (depth != 0)
		return ;
	pos.end = *i;
	tmp = tokenize_word(line, pos.start, pos.end - 1);
	if (!tmp)
		return ;
	add_token(head, tmp, T_GROUP, pos);
	free(tmp);
}

void	tokenize(t_token **head, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '\'')
			tokenize_simple_quote(head, &i, line);
		else if (line[i] == '"')
			tokenize_double_quote(head, &i, line);
		else if (line[i] == '|' || line[i] == '&')
			tokenize_pipe(head, &i, line);
		else if (line[i] == '(')
			tokenize_parenthesis(head, &i, line);
		else if (line[i] == '>')
			tokenize_out(head, &i, line);
		else if (line[i] == '<')
			tokenize_in(head, &i, line);
		else
			tokenize_spaces(head, &i, line);
	}
}
