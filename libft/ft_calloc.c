/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:06:35 by mpagani           #+#    #+#             */
/*   Updated: 2022/11/15 14:35:48 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (size && count > SIZE_MAX / size)
		return (malloc(0));
	ptr = malloc(size * count);
	if (!ptr)
		return (ptr);
	ft_bzero(ptr, size * count);
	return (ptr);
}
