/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelfort <fbelfort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 13:03:40 by mpagani           #+#    #+#             */
/*   Updated: 2023/02/24 16:44:14 by fbelfort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	new_str = ft_calloc(sizeof(char), len + 1);
	if (!new_str)
		return (NULL);
	while (i < len && s[start])
		new_str[i++] = s[start++];
	new_str[i] = '\0';
	return (new_str);
}
