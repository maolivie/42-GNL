/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:45:53 by molivier          #+#    #+#             */
/*   Updated: 2021/01/20 17:10:37 by molivier         ###   ########lyon.fr   */
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
		if ((node->save = ft_strdup("")) == NULL)
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

void	handle_exceptions(ssize_t ret, t_list **alst, t_list *node, char **line)
{
	t_list	*lst;
	t_list	*prev;

	if (ret == 0 || ret == -1)
	{
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
		free(lst->save);
		free(lst);
	}
	if (ret == -1)
	{
		free(*line);
		*line = NULL;
	}
}

ssize_t	get_line(char *src, char **line, t_list *node)
{
	char	*nl_ptr;

	if ((nl_ptr = ft_strchr(src, '\n')))
		*nl_ptr = '\0';
	if (src == node->save)
	{
		if ((*line = ft_strdup(src)) == NULL)
			return (-1);
		if (nl_ptr)
		{
			if ((node->save = ft_strdup(nl_ptr + 1)) == NULL)
				return (-1);
		}
		else
			node->save = NULL;
		free(src);
	}
	else
	{
		if ((*line = ft_strappend(*line, src)) == NULL)
			return (-1);
		if (nl_ptr)
		{
			if ((node->save = ft_strdup(nl_ptr + 1)) == NULL)
				return (-1);
		}
	}
	return (nl_ptr ? 1 : 0);
}

ssize_t	get_line(char *src, char **line, t_list *node)
{
	char	*nl_ptr;

	if ((nl_ptr = ft_strchr(src, '\n')))
		*nl_ptr = '\0';
	if (src == node->save)
	{
		if ((*line = ft_strdup(src)) == NULL)
			return (-1);
		node->save = NULL;
		free(src);
	}
	else
	{
		if ((*line = ft_strappend(*line, src)) == NULL)
			return (-1);
	}
	return (nl_ptr ? 1 : 0);
}

int		get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];
	static t_list	*lst = NULL;
	t_list			*node;
	ssize_t			ret;

	if ((node = get_node(&lst, fd)) == NULL)
		return (-1);
	ret = get_line(node->save, line, node);
	while (ret == 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		ret = get_line(buf, line, node);
	}
	handle_exceptions(ret, &lst, node, line);
	return ((int)ret);
}
