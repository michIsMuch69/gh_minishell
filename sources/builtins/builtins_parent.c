/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 14:32:55 by jedusser         ###   ########.fr       */
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

void handle_sigpipe(int sig)
{
    (void)sig; // Suppress unused parameter warning
    exit(1);
}

int ft_exit(t_data *data, int i, int **fds, int last_fd)
{
    int arg = 0;

    // Check if there are too many arguments
    if (data[i].args.size > 2)
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        return 1;
    }

    // Check if the argument is numeric
    if (data[i].args.size == 2 && data[i].args.tab[1])
    {
        if (!is_numeric_str(data[i].args.tab[1]))
        {
            ft_putstr_fd("exit: numeric argument required\n", 2);
            return 2;
        }
        arg = ft_atoi(data[i].args.tab[1]) % 256;
        if (arg < 0)
            arg += 256;
    }

    // Set up a custom SIGPIPE handler
    signal(SIGPIPE, handle_sigpipe);

    printf("Closing pipes in ft_exit\n");

    // Close file descriptors based on the position in the pipeline
    if (i == 0) // First command
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
    else if (i == data->tab_size - 1) // Last command
    {
        if (last_fd != -1)
        {
            close(last_fd);
            last_fd = -1;
        }
    }
    else // Middle commands
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

    // Free the pipes if they were allocated
    if (fds != NULL && data->tab_size > 1)
    {
        free_pipes(fds, data->tab_size - 1);
    }

    ft_putstr_fd("exit\n", 1);
    free_struct(data, data[i].tab_size);

    exit(arg);

    return 0; // This line will never be reached
}



//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
