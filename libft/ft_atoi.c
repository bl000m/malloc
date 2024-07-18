/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelfort <fbelfort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:55:22 by mpagani           #+#    #+#             */
/*   Updated: 2023/03/06 14:31:56 by fbelfort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int				i;
	long long int	nb;
	long long int	minus;

	i = 0;
	nb = 0;
	minus = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			minus = -1;
	if (str[i] == '-' || str[i] == '+')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - '0';
	nb *= minus;
	if (nb == (int) nb)
		return ((int) nb);
	return (0);
}
