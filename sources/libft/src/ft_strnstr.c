/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 12:20:54 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:26 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	x;

	i = 0;
	x = 0;
	if (ft_strlen(little) == 0)
		return ((char *)big);
	while (i < len && big[i])
	{
		while (big[i] == little[x] && i < len)
		{
			if (little[x + 1] == '\0')
				return ((char *) &big[i - x]);
			x++;
			i++;
		}
		if (x > 0)
		{
			i = i - x;
			x = 0;
		}
		i++;
	}
	return (NULL);
}
