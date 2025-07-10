/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:46:14 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/19 14:22:15 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	s_len;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		result = ft_strdup("");
	else
	{
		if (len > s_len - start)
			len = s_len - start;
		result = (char *)malloc(len + 1);
		if (result == NULL)
			return (NULL);
		while (i < len)
		{
			result[i] = s[start + i];
			i++;
		}
		result[i] = '\0';
	}
	return (result);
}
