/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 15:08:18 by jedusser         ###   ########.fr       */
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
				data->env.tab[j] = NULL;
				data->env.size--;
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

void ft_exit_close_fds(t_data *data, int i, int **fds, int last_fd, int tab_size)
{
    if (i == 0)
    {
        if (fds != NULL && fds[i] != NULL)
        {
            if (fds[i][1] != -1)
            {
                close(fds[i][1]);
                fds[i][1] = -1;
            }
        }
        if (last_fd != -1)
        {
            close(last_fd);
            last_fd = -1;
        }
    }
    else if (i == data->tab_size - 1)
    {
        if (last_fd != -1)
        {
            close(last_fd);
            last_fd = -1;
        }
    }
    else
    {
        if (fds != NULL && fds[i] != NULL)
        {
            if (fds[i][0] != -1)
            {
                close(fds[i][0]);
                fds[i][0] = -1;
            }
            if (fds[i][1] != -1)
            {
                close(fds[i][1]);
                fds[i][1] = -1;
            }
        }
        if (last_fd != -1)
        {
            close(last_fd);
            last_fd = -1;
        }
    }
}



int ft_exit(t_data *data, int i, int **fds, int last_fd)
{
   int	arg;

	arg = 0;
	if (data[i].args.size > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	if (data[i].args.size == 2 && data[i].args.tab[1])
	{
		if (is_numeric_str(data[i].args.tab[1]) == 0)
        {
			ft_putstr_fd("exit: numeric argument required\n", 2);
            arg = 2;
        }
		else
		{
			arg = ft_atoi(data[i].args.tab[1]) % 256;
			if (arg < 0)
				arg += 256;
		}
	}
	ft_exit_close_fds(data, i, fds, last_fd, data->tab_size);
    if (fds != NULL && data->tab_size > 1)
        free_pipes(fds, data->tab_size - 1);
    ft_putstr_fd("exit\n", 1);
    free_struct(data, data[i].tab_size);
    exit(arg);

    return (0);
}



//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
