/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:36:33 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/22 13:31:06 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	int		i;

	i = 0;
  if (!src || !dst)
    return (dst);
  //printf("dst = %s\nsrc = %s\n", dst, src);
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	return (dst);
}
