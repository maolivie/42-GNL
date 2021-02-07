/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:45:53 by molivier          #+#    #+#             */
/*   Updated: 2021/01/28 15:59:30 by molivier         ###   ########lyon.fr   */
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
	{
		if (node)
			free(*line);
		*line = NULL;
	}
	return ((int)ret);
}

ssize_t	save_text_read(t_list *node, char *nl)
{
	char	*save;

	save = ft_strdup(nl + 1);
	if (save == NULL)
		return (-1);
	free(node->save);
	node->save = save;
	return (1);
}

ssize_t	get_line(char **line, const char *src, char **add_nl, t_list *node)
{
	*add_nl = ft_strchr(src, '\n');
	if (*add_nl)
		**add_nl = '\0';
	if (src == node->save)
	{
		if (*add_nl)
		{
			*line = ft_strdup(src);
			if (*line == NULL)
				return (-1);
		}
		else
		{
			*line = node->save;
			node->save = NULL;
		}
	}
	else
	{
		*line = ft_strappend(*line, src);
		if (*line == NULL)
			return (-1);
	}
	return (*add_nl ? 1 : 0);
}

int		get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];
	static t_list	*lst = NULL;
	t_list			*node;
	char			*nl;
	ssize_t			ret;

	if (fd < 0 || line == NULL || BUFFER_SIZE < 1)
		return (handle_exceptions(-1, &lst, NULL, line));
	node = get_node(&lst, fd);
	if (node == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	ret = get_line(line, node->save, &nl, node);
	while (ret == 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		ret = get_line(line, buf, &nl, node);
	}
	if (ret == 1)
		ret = save_text_read(node, nl);
	return (handle_exceptions(ret, &lst, node, line));
}
