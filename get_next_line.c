/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgryshch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 18:43:54 by vgryshch          #+#    #+#             */
/*   Updated: 2018/03/09 17:11:08 by vgryshch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list		*ft_save_string(t_list **list, int fd)
{
	t_list			*go_by_list;
	t_list			*add_to_list;

	go_by_list = *list;
	if (*list)
	{
		while (go_by_list)
		{
			if ((int)go_by_list->content_size == fd)
				return (go_by_list);
			go_by_list = go_by_list->next;
		}
		add_to_list = ft_lstnew("", 1);
		add_to_list->content_size = fd;
		ft_lstadd(list, add_to_list);
	}
	else if (!(*list))
	{
		*list = ft_lstnew("", 1);
		(*list)->content_size = fd;
		return (*list);
	}
	return (*list);
}

static int			ft_cut_string(char *str, char **line)
{
	char			*find_newline;
	char			*find_end_line;

	find_newline = (void*)0;
	find_end_line = (void*)0;
	find_newline = ft_strchr(str, '\n');
	if (*str == '\0')
	{
		*line = (void*)0;
		return (0);
	}
	else if (find_newline == (void*)0)
	{
		find_end_line = ft_strchr(str, '\0');
		*line = ft_strsub(str, 0, (find_end_line - str));
		ft_memmove(str, ft_strchr(str, '\0'), ft_strlen(str));
		free(find_end_line);
		return (1);
	}
	*line = ft_strsub(str, 0, (find_newline - str));
	ft_memmove(str, (ft_strchr(str, '\n') + 1), ft_strlen(find_newline));
	free(find_newline);
	return (1);
}

int					get_next_line(const int fd, char **line)
{
	int				ret;
	char			*buf;
	char			*free_string;
	static t_list	*static_list = (void*)0;
	t_list			*point_to_list;

	buf = (void*)0;
	free_string = (void*)0;
	if (fd < 0 || !line || BUFF_SIZE < 1 || (ret = read(fd, buf, 0)) < 0)
		return (-1);
	buf = ft_memalloc(BUFF_SIZE + 1);
	point_to_list = ft_save_string(&static_list, fd);
	while ((ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = '\0';
		free_string = point_to_list->content;
		point_to_list->content = ft_strjoin(free_string, buf);
		free(free_string);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	free(buf);
	if (ft_cut_string(&*point_to_list->content, line) == 1)
		return (1);
	return (0);
}
