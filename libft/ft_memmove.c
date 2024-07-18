/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 08:12:52 by mpagani           #+#    #+#             */
/*   Updated: 2022/11/14 13:51:05 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*ptr_src;
	unsigned char	*ptr_dst;

	ptr_dst = (unsigned char *)dst;
	ptr_src = (unsigned char *)src;
	if (dst > src)
	{
		while (len--)
			ptr_dst[len] = ptr_src[len];
	}
	else
		ft_memcpy(dst, src, len);
	return ((void *)dst);
}
