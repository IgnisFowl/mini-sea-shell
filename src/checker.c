/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 08:33:40 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:16:04 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pipe_token_loop(const char *line)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (line[i])
	{
		if (line[i] == '|')
		{
			if (!word || line[i + 1] == '\0')
			{
				ft_putstr_fd("Syntax error: invalid pipe usage\n", 2);
				return (-1);
			}
			if (line[i + 1] == '|')
				break ;
			word = 0;
		}
		else if (!is_space(line[i]))
			word = 1;
		i++;
	}
	return (i);
}

int	check_pipes(const char *line)
{
	int	i;

	i = check_pipe_token_loop(line);
	if (i < 0)
		return (0);
	if (line[0] == '|' || line[i - 1] == '|')
	{
		ft_putstr_fd("Syntax error: pipe at start or end\n", 2);
		return (0);
	}
	return (1);
}

static int	skip_quote_content(char *line, int *i, char quote)
{
	while (line[*i])
	{
		if (quote == '"' && line[*i] == '\\' && line[*i + 1])
			(*i) += 2;
		else if (line[*i] == quote)
			return (1);
		else
			(*i)++;
	}
	return (0);
}

int	check_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i++];
			if (!skip_quote_content(line, &i, quote))
			{
				ft_putstr_fd("Syntax error: unmatched quote\n", 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	checker(char *line)
{
	if (!*line)
		return (0);
	if (!check_quotes(line))
		return (0);
	if (!check_brackets(line))
		return (0);
	if (!check_pipes(line))
		return (0);
	if (!check_semicolon(line))
		return (0);
	if (!check_redirections(line))
		return (0);
	if (!check_escapes(line))
		return (0);
	if (!check_parenthesis(line))
		return (0);
	return (1);
}
