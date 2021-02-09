/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/03 18:47:32 by molivier          #+#    #+#             */
/*   Updated: 2021/02/09 15:03:49 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# include <stdlib.h>

typedef struct	s_list
{
	struct s_list	*next;
	char			*data;
	int				fd;
}				t_list;

char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *src);
char			*ft_strappend(char *s1, const char *s2);
void			ft_lstdelone(t_list **alst, t_list *node);

int				get_next_line(int fd, char **line);

#endif
