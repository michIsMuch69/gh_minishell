/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:36:33 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 07:02:27 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	if (!src || !dst)
		return (dst);
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	return (dst);
}
