/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 13:21:39 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/03 15:32:37 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_base(unsigned long long nb, unsigned int base, char *digits)
{
	int	x;

	x = 1;
	if (nb >= base)
	{
		x += ft_putnbr_base(nb / base, base, digits);
	}
	write(1, &digits[nb % base], 1);
	return (x);
}
