/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:41:08 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:57:32 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static int	ft_print_minus(int n, int fd)
{
	ft_putchar_fd('-', fd);
	return (n *= -1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (fd)
	{
		if (n == -2147483648)
		{
			write(fd, "-2147483648", 11);
			return ;
		}
		else if (n < 0)
			n = ft_print_minus(n, fd);
		if (n > 9)
			ft_putnbr_fd((n / 10), fd);
		n = (n % 10) + 48;
		ft_putchar_fd(n, fd);
	}
}
