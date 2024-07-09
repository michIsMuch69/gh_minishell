/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 15:28:18 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:51:15 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static int	check_data(const char **flag, va_list args)
{
	int	count;

	(*flag)++;
	if (**flag == '%')
		count = putcharac('%');
	else if (**flag == 'c')
		count = putcharac(va_arg(args, int));
	else if (**flag == 's')
		count = putstring(va_arg(args, char *));
	else if (**flag == 'p')
		count = put_address(va_arg(args, unsigned long long), 1);
	else if (**flag == 'x' || **flag == 'X')
		count = put_hexa(va_arg(args, unsigned int), &(*flag), 1);
	else if (**flag == 'd' || **flag == 'i')
		count = put_int(va_arg(args, int), 1);
	else if (**flag == 'u')
		count = put_int(va_arg(args, unsigned int), 1);
	else
		count = -1;
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		count;
	int		tmp;
	va_list	lst;

	count = 0;
	va_start(lst, str);
	if (!str)
		return (putstring("(null)"));
	while (*str)
	{
		if (*str == '%')
			tmp = check_data(&str, lst);
		else
			tmp = putcharac(*str);
		if (tmp == -1)
			return (-1);
		count += tmp;
		str++;
	}
	va_end(lst);
	return (count);
}
