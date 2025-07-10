/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 09:06:29 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/19 10:17:08 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tokenize_word(char *line, int start, int len)
{
	char	*tmp;
	int		j;

	j = 0;
	tmp = malloc((len - start) + 1);
	if (!tmp)
		return (NULL);
	while (start < len)
	{
		if (line[start] == '\\' && line[start + 1] < len)
			tmp[j++] = line[++start];
		else
			tmp[j++] = line[start++];
	}
	tmp[j] = '\0';
	if (j > 0 && tmp[j - 1] == '\n')
		tmp[j - 1] = '\0';
	return (tmp);
}

static	t_token_pos	scan_word(const char *line, int i)
{
	t_token_pos	pos;

	pos.start = i;
	while (line[i] && line[i] != ' ' && !is_metachar(line[i]))
	{
		if (line[i] == '\\' && line[i + 1])
			i += 2;
		else if (line[i] == '$' && line[i + 1] == '{')
		{
			i++;
			while (line[i] && line[i] != '}')
				i++;
			if (line[i] == '}')
				i++;
		}
		else
			i++;
	}
	pos.end = i;
	return (pos);
}

void	tokenize_spaces(t_token **head, int *i, char *line)
{
	t_token_pos	pos;
	char		*tmp;

	pos = scan_word(line, *i);
	tmp = tokenize_word(line, pos.start, pos.end);
	if (tmp)
	{
		add_token(head, tmp, T_WORD, pos);
		free(tmp);
	}
	*i = pos.end;
}

void	tokenize_out(t_token **head, int *i, char *line)
{
	t_token_pos	pos;

	pos.start = *i;
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			pos.end = *i;
			add_token(head, ">>", T_REDIR_APPEND, pos);
			(*i) += 2;
		}
		else
		{
			pos.end = *i;
			add_token(head, ">", T_REDIR_OUT, pos);
			(*i)++;
		}
	}
}

void	tokenize_in(t_token **head, int *i, char *line)
{
	t_token_pos	pos;

	pos.start = *i;
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			pos.end = *i;
			add_token(head, "<<", T_HEREDOC, pos);
			(*i) += 2;
		}
		else
		{
			pos.end = *i;
			add_token(head, "<", T_REDIR_IN, pos);
			(*i)++;
		}
	}
}
