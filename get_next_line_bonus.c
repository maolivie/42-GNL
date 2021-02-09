/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 12:24:18 by molivier          #+#    #+#             */
/*   Updated: 2021/02/09 15:05:25 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line_bonus.h"

t_list	*get_node(t_list **alst, int fd)
{
	t_list	*node;

	node = *alst;
	while (node)
	{
		if (node->fd == fd)
			break ;
		node = node->next;
	}
	if (node == NULL)
	{
		node = (t_list *)malloc(sizeof(t_list));
		if (node == NULL)
			return (NULL);
		node->data = ft_strdup("");
		if (node->data == NULL)
		{
			free(node);
			return (NULL);
		}
		node->fd = fd;
		node->next = *alst;
		*alst = node;
	}
	return (node);
}

int		handle_exceptions(ssize_t ret, t_list **alst, t_list *node, char **line)
{
	if (ret == 0 || ret == -1)
		ft_lstdelone(alst, node);
	if (ret == -1 && line)
		*line = NULL;
	return ((int)ret);
}

ssize_t	read_file(t_list *node, int fd)
{
	char	buf[BUFFER_SIZE + 1];
	ssize_t	ret;

	ret = 1;
	while (ft_strchr(node->data, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		node->data = ft_strappend(node->data, buf);
		if (node->data == NULL)
			return (-1);
	}
	return (ret);
}

ssize_t	split_content(char **line, t_list *node)
{
	char	*nl;
	char	*save;

	nl = ft_strchr(node->data, '\n');
	if (nl)
	{
		save = ft_strdup(nl + 1);
		if (save == NULL)
			return (-1);
		*nl = '\0';
		*line = ft_strdup(node->data);
		free(node->data);
		node->data = save;
		if (*line == NULL)
			return (-1);
		return (1);
	}
	else
	{
		*line = node->data;
		node->data = NULL;
		return (0);
	}
}

int		get_next_line(int fd, char **line)
{
	static t_list	*lst = NULL;
	t_list			*node;
	ssize_t			ret;

	if (fd < 0 || line == NULL || BUFFER_SIZE < 1)
		return (handle_exceptions(-1, &lst, NULL, line));
	node = get_node(&lst, fd);
	if (node == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	ret = read_file(node, fd);
	if (ret != -1)
		ret = split_content(line, node);
	return (handle_exceptions(ret, &lst, node, line));
}
