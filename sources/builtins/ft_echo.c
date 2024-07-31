/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:17:03 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 15:37:42 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_echo(t_data *data)
{
	int		flag;
	int		i;
	char	*tmp1;

	flag = 0;
	i = 1;
	if (ft_strcmp(data->args.tab[1], "cd") == 0 \
	&& ft_strcmp(data->args.tab[2], "~") == 0)
	{
		if (ft_getenv("HOME", data->env.tab, &tmp1) == 0)
			return (ft_printf("cd %s\n", tmp1), free(tmp1), 0);
	}
	if (data->args.tab[1] && ft_strncmp(data->args.tab[1], "-n", 2) == 0)
	{
		flag = 1;
		while (data->args.tab[i] && ft_strncmp(data->args.tab[i], "-n", 2) == 0)
			i++;
	}
	while (data->args.tab[i])
	{
		ft_printf("%s", data->args.tab[i]);
		if (data->args.tab[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!flag)
		ft_printf("\n");
	return (0);
}
