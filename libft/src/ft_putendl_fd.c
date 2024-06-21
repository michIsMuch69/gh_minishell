/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:33:21 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:57:28 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	size_t	count;
	char	c;

	count = ft_strlen(s);
	c = '\n';
	if (fd)
	{
		write(fd, s, count);
		write(fd, &c, 1);
	}
}
