/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpagani <mpagani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:37:22 by mpagani           #+#    #+#             */
/*   Updated: 2022/12/27 16:24:42 by mpagani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static t_gnl	*record[OPEN_MAX];
	char			*line;

	line = NULL;
	if (fd == -1 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		ft_free_list(record[fd]);
		record[fd] = NULL;
		return (NULL);
	}
	read_stock(&record[fd], fd);
	if (!record[fd])
		return (NULL);
	compose_line(&line, record[fd]);
	clean_record(&record[fd]);
	if (line[0] == '\0')
	{
		ft_free_list(record[fd]);
		record[fd] = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}

void	read_stock(t_gnl **record, int fd)
{
	char	buffer[BUFFER_SIZE + 1];
	int		characters;

	characters = 1;
	while (check_eol(*record) == 0 && characters != 0)
	{
		characters = read(fd, buffer, BUFFER_SIZE);
		if (characters == -1 || ((*record) == NULL && characters == 0))
		{
			buffer[0] = '\0';
			return ;
		}
		buffer[characters] = '\0';
		ft_add_pattern(record, buffer, characters);
	}
}

void	compose_line(char **line, t_gnl *record)
{
	int		i;
	int		j;

	j = 0;
	*line = malloc(sizeof(char) * (ft_line_size(record) + 1));
	if (!*line || !record)
		return ;
	while (record)
	{
		i = 0;
		while (record->content[i])
		{
			if (record->content[i] == '\n')
			{
				(*line)[j++] = record->content[i];
				break ;
			}
			(*line)[j++] = record->content[i++];
		}
		record = record->next;
	}
	(*line)[j] = '\0';
}

void	clean_record(t_gnl **record)
{
	t_gnl	*last;
	t_gnl	*temp;
	int		i;
	int		t;

	i = 0;
	t = 0;
	temp = malloc(sizeof(*temp));
	if (!temp || (!*record))
		return ;
	temp->next = NULL;
	last = ft_gnllast(*record);
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content[i] && last->content[i] == '\n')
		i++;
	temp->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i) + 1));
	if (!temp->content)
		return ;
	while (last->content[i])
		temp->content[t++] = last->content[i++];
	temp->content[t] = '\0';
	ft_free_list(*record);
	*record = temp;
}

void	ft_free_list(t_gnl *record)
{
	t_gnl	*current;
	t_gnl	*following;

	current = record;
	while (current)
	{
		free(current->content);
		following = current->next;
		free(current);
		current = following;
	}
}
