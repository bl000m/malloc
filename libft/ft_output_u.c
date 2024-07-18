/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 15:39:14 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/03 15:33:44 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_output_u(va_list args, int size)
{
	unsigned int	res;

	res = va_arg(args, unsigned int);
	size += ft_putnbr_base(res, 10, "0123456789");
	return (size);
}
