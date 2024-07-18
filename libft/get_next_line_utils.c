/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:37:26 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/27 16:28:22 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_eol(t_gnl *record)
{
	int		i;
	t_gnl	*ptr;

	if (!record)
	{
		return (0);
	}
	i = 0;
	ptr = ft_gnllast(record);
	while (ptr->content[i] != '\0')
	{
		if (ptr->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_add_pattern(t_gnl **lst, char *buffer, int characters)
{
	t_gnl	*ptr;
	t_gnl	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(*new));
	if (!new)
		return ;
	new->next = NULL;
	new->content = malloc(sizeof(char) * (characters + 1));
	if (!new->content)
		return ;
	while (buffer[i] && i < characters)
	{
		new->content[i] = buffer[i];
		i++;
	}
	new->content[i] = '\0';
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = ft_gnllast(*lst);
	ptr->next = new;
}

int	ft_line_size(t_gnl *record)
{
	int	len;
	int	i;

	len = 0;
	while (record)
	{
		i = 0;
		while (record->content[i] != '\0')
		{
			if (record->content[i] == '\n')
			{
				len++;
				break ;
			}
			len++;
			i++;
		}
		record = record->next;
	}
	return (len);
}

t_gnl	*ft_gnllast(t_gnl *lst)
{
	t_gnl	*ptr;

	ptr = lst;
	if (ptr == NULL)
		return (NULL);
	while (ptr->next != NULL)
		ptr = ptr->next;
	return (ptr);
}
