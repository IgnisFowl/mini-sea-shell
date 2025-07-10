/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:38:21 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/21 11:44:08 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_simple_quote(t_token **head, int *i, char *line)
{
	t_token_pos	pos;
	char		*tmp;
	int			start;

	pos.start = *i;
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	pos.end = *i;
	tmp = ft_substr(line, start, pos.end - start);
	if (tmp != NULL)
	{
		add_token(head, tmp, T_SQUOTE, pos);
		free(tmp);
	}
	if (line[*i] == '\'')
		(*i)++;
}

static int	handle_escaped(const char *line, int *i, int *escaped)
{
	if (line[*i] == '\\')
	{
		*escaped = !*escaped;
		(*i)++;
		return (1);
	}
	*escaped = 0;
	return (0);
}

static char	*extract_escaped(char *line, int start, int end)
{
	int		i;
	int		j;
	char	*res;

	res = malloc((end - start) + 1);
	if (!res)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		if (line[i] == '\\' && line[i + 1] && ft_strchr("\"\\$`", line[i + 1]))
		{
			res[j++] = line[i + 1];
			i += 2;
		}
		else
			res[j++] = line[i++];
	}
	res[j] = '\0';
	return (res);
}

void	tokenize_double_quote(t_token **head, int *i, char *line)
{
	t_token_pos	pos;
	char		*tmp;
	int			escaped;
	int			start;

	pos.start = *i;
	(*i)++;
	start = *i;
	escaped = 0;
	while ((line[*i] && line[*i] != '"' ) || escaped)
	{
		if (!handle_escaped(line, i, &escaped))
			(*i)++;
	}
	pos.end = *i + 1;
	tmp = extract_escaped(line, start, pos.end - 1);
	add_token(head, tmp, T_DQUOTE, pos);
	free(tmp);
	if (line[*i] == '"')
		(*i)++;
}
