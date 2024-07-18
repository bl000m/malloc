/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 09:40:02 by mpagani           #+#    #+#             */
/*   Updated: 2022/11/15 12:18:54 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;

	if ((!dest || !src) && size == 0)
		return (0);
	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	i = 0;
	if (size > dest_len)
	{
		while (src[i] && ((dest_len + i) < (size - 1)))
		{
			dest[dest_len + i] = src[i];
			i++;
		}
		dest[dest_len + i] = '\0';
		return (src_len + dest_len);
	}
	return (src_len + size);
}
