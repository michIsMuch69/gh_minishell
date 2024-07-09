/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:50:04 by fberthou          #+#    #+#             */
/*   Updated: 2024/03/04 20:35:48 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static int	ft_numlen(int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n = n * -1;
		i++;
	}
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i + 1);
}

static void	fill_table(int n, int numlen, char *dst, int sign)
{
	char	temp;

	while (n > 9)
	{
		temp = n % 10 + 48;
		dst[numlen - 1] = temp;
		n /= 10;
		numlen--;
	}
	if (sign == -1)
	{
		dst[1] = n + 48;
		dst[0] = '-';
	}
	else
		dst[0] = n + 48;
}

char	*ft_itoa(int n)
{
	int		numlen;
	int		sign;
	char	*dst;

	if (n == -2147483648)
	{
		dst = ft_substr("-2147483648", 0, 11);
		return (dst);
	}
	sign = 1;
	numlen = ft_numlen(n);
	if (n < 0)
	{
		n = n * -1;
		sign = -1;
	}
	dst = malloc(numlen + 1 * sizeof(char));
	if (!dst)
		return (NULL);
	fill_table(n, numlen, dst, sign);
	dst[numlen] = '\0';
	return (dst);
}
