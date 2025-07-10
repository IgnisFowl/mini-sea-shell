/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:51:41 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 11:32:25 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_special_vars(char *res, const char *str, int *i)
{
	char	*tmp;

	if (str[*i] == '?')
	{
		tmp = ft_itoa(g_last_signal);
		if (tmp)
		{
			res = ft_strjoin_free(res, tmp);
			free(tmp);
		}
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		tmp = ft_itoa(getpid());
		if (tmp)
		{
			res = ft_strjoin_free(res, tmp);
			free(tmp);
		}
		(*i)++;
	}
	else
		res = append_char(res, '$');
	return (res);
}

static char	*handle_variables_exp(char *res, \
	const char *str, t_env *env, int *i)
{
	int		start;
	int		len;
	char	varname[256];
	char	*value;

	if (str[*i] == '{')
	{
		(*i)++;
		start = *i;
		len = get_braced_var_length(str, i);
		if (str[*i] == '}' && len < 256)
		{
			ft_strlcpy(varname, str + start, len + 1);
			value = get_env_value(env, varname);
			res = ft_strjoin_free(res, value);
			(*i)++;
		}
	}
	else
	{
		res = expand_and_append(res, str, env, i);
		(*i)++;
	}
	return (res);
}

static int	is_special_char(const char *str, int i)
{
	char	c;

	c = str[i + 1];
	if (c == '\0')
		return (1);
	if (ft_isalpha(c) || c == '_' || c == '?' || \
	c == '$' || c == '{' || ft_isdigit(c))
		return (0);
	return (1);
}

static void	expand_loop(const char *str, t_env *env, char **res, int *i)
{
	if (str[*i] == '$')
	{
		if (is_special_char(str, *i))
		{
			*res = append_char(*res, '$');
			(*i)++;
		}
		else
		{
			(*i)++;
			if (str[*i] == '?' || str[*i] == '$')
				*res = append_special_vars(*res, str, i);
			else if (ft_isdigit(str[*i]))
				(*i)++;
			else
				*res = handle_variables_exp(*res, str, env, i);
		}
	}
	else
	{
		*res = append_char(*res, str[*i]);
		(*i)++;
	}
}

char	*expand_variables(const char *str, t_env *env)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
		expand_loop(str, env, &res, &i);
	return (res);
}
