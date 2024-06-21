/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:18:30 by fberthou          #+#    #+#             */
/*   Updated: 2024/01/04 15:09:19 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	clear_buffer(char *buffer, int index)
{
	while (index < BUFFER_SIZE)
		buffer[index++] = '\0';
}

size_t	lenstr(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_calloct(size_t nmemb, size_t size)
{
	char	*dst;
	size_t	i;
	size_t	result;

	result = nmemb * size;
	if (result == 0 || (result / nmemb != size))
		return (NULL);
	dst = malloc(result);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < result)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

char	*cpy_substr(char *buffer)
{
	char	*dst;
	size_t	len;
	size_t	i;

	len = lenstr(buffer);
	dst = ft_calloct(sizeof(char), len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dst[i] = buffer[i];
		i++;
	}
	return (dst);
}

char	*concate_buffer(char *buffer, char *tmp)
{
	size_t	i;
	size_t	x;
	char	*dst;

	dst = ft_calloct(sizeof(char), (lenstr(tmp) + lenstr(buffer) + 1));
	if (!dst)
		return (free(tmp), NULL);
	i = 0;
	while (tmp[i])
	{
		dst[i] = tmp[i];
		i++;
	}
	x = 0;
	while (buffer[x])
	{
		dst[i + x] = buffer[x];
		x++;
	}
	free(tmp);
	return (dst);
}
