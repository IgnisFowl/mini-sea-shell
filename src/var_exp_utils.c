/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:02:10 by aarie-c2          #+#    #+#             */
/*   Updated: 2025/06/28 11:25:15 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*append_char(char *res, char c)
{
	int		len;
	char	*new;

	len = ft_strlen(res);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	ft_strlcpy(new, res, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	free(res);
	return (new);
}

int	get_braced_var_length(const char *str, int *i)
{
	int	len;

	len = 0;
	while (str[*i] && str[*i] != '}')
	{
		len++;
		(*i)++;
	}
	return (len);
}

int	get_var_length(const char *str)
{
	int	i;

	if (!is_valid_varchar(str[0], 1))
		return (0);
	i = 1;
	while (str[i] && is_valid_varchar(str[i], 0))
		i++;
	return (i);
}

char	*expand_and_append(char *res, const char *str, t_env *env, int *i)
{
	char	varname[256];
	char	*value;
	int		len;

	len = get_var_length(str + *i);
	if (len >= 256 || len == 0)
		return (res);
	ft_strlcpy(varname, str + *i, len + 1);
	value = get_env_value(env, varname);
	res = ft_strjoin_free(res, value);
	*i += len - 1;
	return (res);
}
