/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:00:59 by florian           #+#    #+#             */
/*   Updated: 2024/07/30 09:05:44 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int		manage_redirection(t_data *data, int i, int **fd, int last_read);
int		close_in_out_files(t_data *data);

void	close_unused_pipes(t_data *data, int i, int **fd)
{
	int	y;

	if (i == 0 || i < data->tab_size - 1)
	{
		close(fd[i][0]);
		y = i + 1;
		while (y < data->tab_size - 1)
		{
			close(fd[y][0]);
			close(fd[y][1]);
			y++;
		}
	}
}

void	close_used_pipes(t_data *data, int i, int **fd, int last_read)
{
	if (i == 0)
		close(fd[i][1]);
	else if (i < data->tab_size - 1)
	{
		close(last_read);
		close(fd[i][1]);
	}
	else
		close(last_read);
	while (i < data->tab_size)
	{
		close_in_out_files(&data[i]);
		i++;
	}
}

void	exec_builtin_child(t_data *data, int i, int **fd, int last_read)
{
	int	exit_status;

	manage_redirection(data, i, fd, last_read);
	close_unused_pipes(data, i, fd);
	exit_status = exec_builtin(data, i, fd, last_read);
	close_used_pipes(data, i, fd, last_read);
	free_pipes(fd, data->tab_size - 1);
	free_struct(data, data->tab_size);
	exit(exit_status);
}
