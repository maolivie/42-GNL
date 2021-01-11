/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 20:27:34 by molivier          #+#    #+#             */
/*   Updated: 2020/12/22 18:44:36 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	get_next_line(int fd, char **line)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*nl_ptr;
	char		*tmp;
	ssize_t		ret;

	if (line == NULL || fd < 0 || BUFFER_SIZE <= 0)
		return (-1);
	if ((nl_ptr = ft_strchr(buf, '\n')))
	{
		*nl_ptr = '\0';
		*line = ft_strdup(buf);
		ft_strlcpy(buf, nl_ptr + 1, BUFFER_SIZE + 1);
		return (1);
	}
	*line = ft_strdup(buf);
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		tmp = *line;
		if ((nl_ptr = ft_strchr(buf, '\n')))
		{
			*nl_ptr = '\0';
			*line = ft_strjoin(*line, buf);
			free(tmp);
			ft_strlcpy(buf, nl_ptr + 1, BUFFER_SIZE + 1);
			return (1);
		}
		*line = ft_strjoin(*line, buf);
		free(tmp);
	}
	if (ret == -1)
	{
		free(*line);
		*line = NULL;
	}
	if (ret == 0)
		*buf = '\0';
	return (ret);
}
