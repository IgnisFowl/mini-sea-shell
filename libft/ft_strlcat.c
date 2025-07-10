/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:25:12 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2024/10/28 11:56:54 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strnlen(const char *c, size_t maxsize)
{
	size_t	i;

	i = 0;
	while (c[i] != '\0' && i < maxsize)
		i++;
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;

	d_len = ft_strnlen(dst, size);
	s_len = ft_strlen(src);
	if (d_len == size)
		return (size + s_len);
	if (s_len < size - d_len)
		ft_memcpy(dst + d_len, src, s_len + 1);
	else
	{
		ft_memcpy(dst + d_len, src, size - d_len - 1);
		dst[size - 1] = '\0';
	}
	return (d_len + s_len);
}
