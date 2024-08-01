/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:19:42 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:02:02 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

ssize_t	ft_putstr_fd(char *s, int fd)
{
	size_t	count;

	count = ft_strlen(s);
	if (fd)
		return (write(fd, s, count));
	return (0);
}
