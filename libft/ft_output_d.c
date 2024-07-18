/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output_d.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 15:23:55 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/03 15:33:37 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_output_d(va_list args, int size)
{
	int	res;

	res = va_arg(args, int);
	ft_putnbr(res);
	if (res == 0)
		size++;
	if (res < 0)
		size++;
	while (res != 0)
	{
		res /= 10;
		size ++;
	}
	return (size);
}
