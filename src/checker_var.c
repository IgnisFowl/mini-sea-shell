/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 09:40:57 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:03:29 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_varname(const char *str, int len)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (!is_valid_varchar(str[0], 1))
		return (0);
	while (i < len)
	{
		if (!is_valid_varchar(str[i], i == 0))
			return (0);
		i++;
	}
	return (1);
}

int	valid_declaration(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=' || i == 0)
		return (0);
	if (!is_valid_varname(str, i))
	{
		ft_putstr_fd("Syntax error: invalid variable name\n", 2);
		return (0);
	}
	return (1);
}

int	valid_dollar_variable(const char *str, int i)
{
	int	start;

	if (str[i] == '{')
	{
		start = i + 1;
		if (str[start] == '}')
			return (0);
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] != '}')
			return (0);
		if (!is_valid_varname(str + start, i - start))
			return (0);
	}
	else
	{
		if (str[i] == '?' || str[i] == '$')
			return (1);
		if (!is_valid_varchar(str[i], 1))
			return (0);
	}
	return (1);
}

int	handle_variable(const char *str, int *i, int dquote)
{
	if (*i > 0 && str[*i - 1] == '\\' && dquote)
		return (1);
	if (str[*i + 1] == '\0' || (dquote && str[*i + 1] == '"'))
		return (1);
	(*i)++;
	if (!valid_dollar_variable(str, *i))
		return (1);
	return (1);
}

int	check_variable(const char *str)
{
	int	i;
	int	squote;
	int	dquote;

	i = 0;
	squote = 0;
	dquote = 0;
	while (str[i])
	{
		check_quotes_state(str[i], &squote, &dquote);
		if (str[i] == '$' && !squote)
		{
			if (!handle_variable(str, &i, dquote))
				return (0);
		}
		i++;
	}
	return (1);
}
