/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 08:59:36 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/04 11:54:06 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_elem;

	new_elem = malloc(sizeof(*new_elem));
	if (!new_elem)
		return (NULL);
	new_elem->content = content;
	new_elem->next = NULL;
	return (new_elem);
}
