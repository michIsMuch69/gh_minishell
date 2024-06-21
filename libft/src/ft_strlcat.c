/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:07:41 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:04 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	x;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	x = 0;
	i = dst_len;
	while (i < size - 1 && src[x])
	{
		dst[i] = src[x];
		i++;
		x++;
	}
	dst[i] = '\0';
	if (size < dst_len)
		return (src_len + size);
	else
		return (dst_len + src_len);
}
