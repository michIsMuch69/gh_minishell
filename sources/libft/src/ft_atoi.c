/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:06:28 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:35 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static	int	skip_spaces(const char *nptr)
{
	int	i;

	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	return (i);
}

int	check_overflow(long result, const char *nptr, size_t index, int sign)
{
	long	max;

	max = 9223372036854775807;
	if (result > max / 10 || (result == max / 10 && nptr[index] - '0' > 7))
	{
		if (sign == -1)
			return (0);
		else
			return (-1);
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int		sign;
	long	result;
	size_t	i;

	result = 0;
	sign = 1;
	i = skip_spaces(nptr);
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (check_overflow(result, nptr, i, sign) == -1)
			return (-1);
		else if (!check_overflow(result, nptr, i, sign))
			return (0);
		else
			result = result * 10 + (nptr[i] - 48);
		i++;
	}
	return (result * sign);
}
