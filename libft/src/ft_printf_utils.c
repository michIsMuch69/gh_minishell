/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 15:09:31 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:50:48 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

int	putcharac(char c)
{
	return (write(1, &c, 1));
}

int	putstring(char *str)
{
	int	length;

	length = 0;
	if (!str)
		return (putstring("(null)"));
	while (str[length])
		length++;
	return (write(1, str, length));
}

int	put_int(long nbr, int firstCall)
{
	static int	count;
	int			tmp;
	int			nb;

	if (firstCall)
		count = 0;
	if (nbr < 0)
	{
		if (putcharac('-') == -1)
			return (-1);
		count += 1;
		nbr *= -1;
	}
	if (nbr > 9)
		put_int(nbr / 10, 0);
	nb = nbr % 10 + '0';
	tmp = write(1, &nb, 1);
	if (tmp == -1)
		return (-1);
	count += tmp;
	return (count);
}

int	put_address(unsigned long long p, int firstCall)
{
	static int	count;
	int			nb;

	if (firstCall)
		count = 0;
	if (!p)
		return (putstring("(nil)"));
	if (p > 15)
		put_address(p / 16, 0);
	nb = p % 16;
	if (nb > 9)
		nb += 'W';
	else
		nb += '0';
	if (p < 16)
	{
		if (putstring("0x") == -1)
			return (-1);
		count += 2;
	}
	if (putcharac(nb) == -1)
		return (-1);
	count += 1;
	return (count);
}

int	put_hexa(unsigned int x, const char **flag, int firstCall)
{
	static int	count;
	int			nb;

	if (firstCall)
		count = 0;
	if (x > 15)
		put_hexa(x / 16, flag, 0);
	nb = x % 16;
	if (nb > 9 && **flag == 'x')
		nb += 'W';
	else if (nb > 9 && **flag == 'X')
		nb += '7';
	else
		nb += '0';
	if (putcharac(nb) == -1)
		return (-1);
	count += 1;
	return (count);
}
