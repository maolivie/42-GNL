/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molivier <molivier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 23:05:15 by molivier          #+#    #+#             */
/*   Updated: 2021/02/09 15:03:42 by molivier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		++len;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	if ((char)c == '\0')
		return ((char *)s + ft_strlen(s));
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		++i;
	}
	return (NULL);
}

char	*ft_strdup(const char *src)
{
	char	*dst;
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	dst = (char *)malloc(sizeof(char) * (len + 1));
	if (dst == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		++i;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strappend(char *s1, const char *s2)
{
	char	*str;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str)
	{
		i = 0;
		while (s1[i])
		{
			str[i] = s1[i];
			++i;
		}
		j = 0;
		while (s2[j])
		{
			str[i + j] = s2[j];
			++j;
		}
		str[len] = '\0';
	}
	free(s1);
	return (str);
}

void	ft_lstdelone(t_list **alst, t_list *node)
{
	t_list	*lst;
	t_list	*prev;

	if (*alst == NULL || node == NULL)
		return ;
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
	free(lst->data);
	free(lst);
}
