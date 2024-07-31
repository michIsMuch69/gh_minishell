/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 15:16:48 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_unset_env(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < data->args.size)
	{
		j = 0;
		while (data->env.tab[j])
		{
			if (ft_strncmp(data->env.tab[j], data->args.tab[i],
					ft_strlen(data->args.tab[i])) == 0)
			{
				free(data->env.tab[j]);
				j--;
				while (data->env.tab[j++])
					data->env.tab[j] = data->env.tab[j + 1];
				data->env.size--;
				data->env.tab[j] = NULL;
				break ;
			}
			j++;
		}
		i++;
	}
}

void	ft_unset_export(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < data->args.size)
	{
		j = 0;
		while (data->export.tab[j])
		{
			if (ft_strncmp(data->export.tab[j], data->args.tab[i],
					ft_strlen(data->args.tab[i])) == 0)
			{
				free(data->export.tab[j]);
				j--;
				while (data->export.tab[j++])
					data->export.tab[j] = data->export.tab[j + 1];
				data->export.tab[j] = NULL;
				data->export.size--;
				break ;
			}
			j++;
		}
		i++;
	}
}

int	ft_unset(t_data *data)
{
	ft_unset_env(data);
	ft_unset_export(data);
	return (0);
}

//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
