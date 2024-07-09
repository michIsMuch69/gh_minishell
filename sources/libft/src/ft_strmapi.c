/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:27:18 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:17 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*dst;

	i = 0;
	dst = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!dst || !s)
		return (NULL);
	while (s[i])
	{
		dst[i] = (*f)(i, s[i]);
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
