/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:48:47 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/03 15:32:04 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_output_p(va_list args, int size)
{
	unsigned long long	res;

	res = (va_arg(args, unsigned long long));
	write(1, "0x", 2);
	size += ft_putnbr_base(res, 16, "0123456789abcdef");
	return (size + 2);
}
