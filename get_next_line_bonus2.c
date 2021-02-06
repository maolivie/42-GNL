/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 12:24:18 by molivier          #+#    #+#             */
/*   Updated: 2021/02/06 20:43:01 by molivier         ###   ########lyon.fr   */
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
		node->save = ft_strdup("");
		if (node->save == NULL)
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

ssize_t	split_content(char **line, t_list *node)
{
	char	*save;
	char	*nl;

	nl = ft_strchr(node->save, '\n');
	if (nl)
	{
		save = ft_strdup(nl + 1);
		if (save == NULL)
			return (-1);
		*nl = '\0';
		*line = ft_strdup(node->save);
		free(node->save);
		node->save = save;
		if (*line == NULL)
			return (-1);
		return (1);
	}
	else
	{
		*line = node->save;
		node->save = NULL;
		return (0);
	}
}

int		get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];
	static t_list	*lst = NULL;
	t_list			*node;
	ssize_t			ret;

	if (fd < 0 || line == NULL || BUFFER_SIZE < 1)
		return (handle_exceptions(-1, &lst, NULL, line));
	node = get_node(&lst, fd);
	if (node == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	ret = 0;
	while (ft_strchr(node->save, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		node->save = ft_strappend(node->save, buf);
		if (node->save == NULL)
			return (handle_exceptions(-1, &lst, node, line));
	}
	if (ret != -1)
		ret = split_content(line, node);
	return (handle_exceptions(ret, &lst, node, line));
}
