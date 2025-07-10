/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nade-lim <nade-lim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:53:50 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/26 17:44:51 by nade-lim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir_syntax(const char *line, int *i, char redir)
{
	if (line[*i] == redir)
		(*i)++;
	if (line[*i] == redir || line[*i] == '>' || line[*i] == '<')
	{
		ft_putstr_fd("Syntax error: too many redirectors\n", 2);
		return (0);
	}
	while (line[*i] && is_space(line[*i]))
			(*i)++;
	if (!line[*i] || line[*i] == '|' || line[*i] == '>' || line[*i] == '<')
	{
		ft_putstr_fd("Syntax error: missing target\n", 2);
		return (0);
	}
	return (1);
}

int	check_redirections(const char *line)
{
	char	redir;
	int		i;
	int		squote;
	int		dquote;

	i = 0;
	squote = 0;
	dquote = 0;
	while (line[i])
	{
		check_quotes_state(line[i], &squote, &dquote);
		if ((line[i] == '>' || line[i] == '<') && (!squote && !dquote))
		{
			redir = line[i++];
			if (!check_redir_syntax(line, &i, redir))
				return (0);
		}
		else
			i++;
	}
	return (1);
}
