/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:17:24 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 15:37:52 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_env(t_data *data)
{
	int	i;

	i = 0;
	if (data->args.size > 1)
		return (127);
	while (data->env.tab[i])
	{
		ft_printf("%s\n", data->env.tab[i]);
		i++;
	}
	return (0);
}
