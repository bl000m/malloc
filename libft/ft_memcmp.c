/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:09:39 by mpagani           #+#    #+#             */
/*   Updated: 2023/03/09 00:50:20 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;
	char	*ptr_s1;
	char	*ptr_s2;

	i = 0;
	ptr_s1 = (char *)s1;
	ptr_s2 = (char *)s2;
	if (n == 0)
		return (0);
	while (ptr_s1[i] && ptr_s2[i] && (ptr_s1[i] == ptr_s2[i]) && i < n - 1)
		i++;
	return ((unsigned char)ptr_s1[i] - (unsigned char)ptr_s2[i]);
}
