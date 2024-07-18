/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:09:38 by mpagani           #+#    #+#             */
/*   Updated: 2022/11/15 10:40:41 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(long int n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	ft_negative(long int *n, char **str)
{
	if (*n == 0)
	{
		*str = ft_calloc(2, sizeof(char));
		*str[0] = '0';
	}
	else if (*n < 0)
	{
		*str[0] = '-';
		*n *= -1;
	}
}

char	*ft_itoa(int n)
{
	char			*str;
	long int		nbr;
	int				digits;

	nbr = n;
	digits = ft_count(nbr);
	str = malloc(digits * sizeof(char) + 1);
	if (!str)
		return (NULL);
	str[digits--] = 0;
	if (nbr <= 0)
		ft_negative(&nbr, &str);
	while (nbr > 0)
	{
		str[digits--] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	return (str);
}
