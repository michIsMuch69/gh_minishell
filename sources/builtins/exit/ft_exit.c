/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:31:28 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 16:35:59 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include "exec.h"

void ft_exit_close_first_last_fds(int i, int **fds, int last_fd)
{
    if (i == 0 && fds != NULL && fds[i] != NULL && fds[i][1] != -1)
    {
        close(fds[i][1]);
        fds[i][1] = -1;
    }
    if (last_fd != -1)
    {
        close(last_fd);
        last_fd = -1;
    }
}

void ft_exit_close_middle_fds(int i, int **fds, int last_fd)
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

void ft_exit_close_fds(t_data *data, int i, int **fds, int last_fd, int tab_size)
{
    if (i == 0 || i == data->tab_size - 1)
        ft_exit_close_first_last_fds(i, fds, last_fd);
    else
        ft_exit_close_middle_fds(i, fds, last_fd);
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