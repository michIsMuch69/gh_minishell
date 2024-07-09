/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:44:51 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:42 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static size_t	lenstr(char const *s, unsigned int start)
{
	size_t	count;

	count = 0;
	if (start <= ft_strlen(s))
	{
		while (s[start])
		{
			count++;
			start++;
		}
	}
	return (count);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	total_len;
	char	*dst;

	total_len = lenstr(s, start);
	if (len > total_len)
		len = total_len;
	dst = ft_calloc(sizeof(char), (len + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len && s[start])
	{
		dst[i] = s[start];
		i++;
		start++;
	}
	return (dst);
}
