/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newdraft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:45:53 by molivier          #+#    #+#             */
/*   Updated: 2021/01/11 18:03:11 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_list	*get_node(t_list **alst, int fd)
{
	t_list	*node;

	if (fd < 0)
		return (NULL);
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
		node->rest = ft_strdup("");
		if (node->rest == NULL)
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

int	handle_exceptions(int ret, t_list **alst, t_list *node, char **line)
{
	t_list	*lst;
	t_list	*prev;

	lst = *alst;
	while (lst != node)
	{
		prev = lst;
		lst = lst->next;
	}
	if (lst == *alst)
		*alst = lst->next;
	else
		prev->next = lst->next;
	free(lst->rest);
	free(lst);
	if (ret == -1)
	{
		free(*line);
		*line = NULL;
	}
	return (ret);
}

int		get_next_line(int fd, char **line)
{
	static t_list	*lst = NULL;
	t_list			*node;
	char			*nl_ptr;
	char			*tmp;
	ssize_t			ret;
	char			buf[BUFFER_SIZE + 1];

	node = get_node(&lst, fd);
	if (node == NULL)
		return (-1);
	nl_ptr = ft_strchr(node->rest, '\n');
	if (nl_ptr)
	{
		*nl_ptr = '\0';
		*line = ft_strdup(node->rest);
		if (*line == NULL)
			return (handle_exceptions(-1, &lst, node, line));
		tmp = node->rest;
		node->rest = ft_strdup(nl_ptr + 1);
		free(tmp);
		if (node->rest == NULL)
			return (handle_exceptions(-1, &lst, node, line));
		return (1);
	}
	*line = ft_strdup(node->rest);
	if (*line == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	free(node->rest);
	node->rest = NULL;
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		tmp = *line;
		nl_ptr = ft_strchr(buf, '\n');
		if (nl_ptr)
		{
			*nl_ptr = '\0';
			*line = ft_strjoin(tmp, buf);
			free(tmp);
			if (*line == NULL)
				return (handle_exceptions(-1, &lst, node, line));
			tmp = node->rest;
			node->rest = ft_strdup(nl_ptr + 1);
			free(tmp);
			if (node->rest == NULL)
				return (handle_exceptions(-1, &lst, node, line));
			return (1);
		}
		*line = ft_strjoin(tmp, buf);
		free(tmp);
		if (*line == NULL)
			return (handle_exceptions(-1, &lst, node, line));
	}
	return (handle_exceptions(ret, &lst, node, line));
}
