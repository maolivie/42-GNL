/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 20:27:34 by molivier          #+#    #+#             */
/*   Updated: 2021/01/28 16:25:05 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

int	handle_exceptions(ssize_t ret, char *nl_ptr, char *buf, char **line)
{
	if (nl_ptr)
		ft_strlcpy(buf, nl_ptr + 1, BUFFER_SIZE + 1);
	else if (ret == 0)
		*buf = '\0';
	else if (ret == -1 && line)
	{
		if (buf)
			free(*line);
		*line = NULL;
	}
	return (nl_ptr ? 1 : (int)ret);
}

int	get_next_line(int fd, char **line)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*nl_ptr;
	ssize_t		ret;

	if (fd < 0 || line == NULL || BUFFER_SIZE < 1)
		return (handle_exceptions(-1, NULL, NULL, line));
	if ((nl_ptr = ft_strchr(buf, '\n')))
		*nl_ptr = '\0';
	if ((*line = ft_strdup(buf)) == NULL)
		return (-1);
	while (nl_ptr == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == 0 || ret == -1)
			break ;
		buf[ret] = '\0';
		if ((nl_ptr = ft_strchr(buf, '\n')))
			*nl_ptr = '\0';
		if ((*line = ft_strappend(*line, buf)) == NULL)
			return (-1);
	}
	return (handle_exceptions(ret, nl_ptr, buf, line));
}
