/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:45:53 by molivier          #+#    #+#             */
/*   Updated: 2021/01/20 10:44:50 by molivier         ###   ########lyon.fr   */
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
		if ((node = (t_list *)malloc(sizeof(t_list))) == NULL)
			return (NULL);
		if ((node->rest = ft_strdup("")) == NULL)
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
	ssize_t			ret;
	char			buf[BUFFER_SIZE + 1];

	if ((node = get_node(&lst, fd)) == NULL)
		return (-1);
	if ((nl_ptr = ft_strchr(node->rest, '\n')))
	{
		*nl_ptr = '\0';
		if ((*line = ft_strdup(node->rest)) == NULL)
			return (handle_exceptions(-1, &lst, node, line));
		if (ft_strreplace(&(node->rest), nl_ptr + 1) == NULL)
			return (handle_exceptions(-1, &lst, node, line));
		return (1);
	}
	if ((*line = ft_strdup(node->rest)) == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	ft_strdel(&(node->rest));
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if ((nl_ptr = ft_strchr(buf, '\n')))
		{
			*nl_ptr = '\0';
			if (ft_strappend(line, buf) == NULL)
				return (handle_exceptions(-1, &lst, node, line));
			if ((node->rest = ft_strdup(nl_ptr + 1)) == NULL)
				return (handle_exceptions(-1, &lst, node, line));
			return (1);
		}
		if (ft_strappend(line, buf) == NULL)
			return (handle_exceptions(-1, &lst, node, line));
	}
	return (handle_exceptions(ret, &lst, node, line));
}

int		get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];
	static t_list	*lst = NULL;
	t_list			*node;
	char			*nl_ptr;
	ssize_t			ret;

	if ((node = get_node(&lst, fd)) == NULL)
		return (-1);
	if ((nl_ptr = ft_strchr(node->text, '\n')))
		*nl_ptr = '\0';
	if ((*line = ft_strdup(node->text)) == NULL)
		return (handle_exceptions(-1, &lst, node, line));
	while (nl_ptr == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		if ((nl_ptr = ft_strchr(buf, '\n')))
			*nl_ptr = '\0';
		if ((*line = ft_strappend(*line, buf)) == NULL)
			return (handle_exceptions(-1, &lst, node, line));
	}
	return (handle_exceptions(ret, prout));
}
