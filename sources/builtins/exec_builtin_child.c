/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:00:59 by florian           #+#    #+#             */
/*   Updated: 2024/07/29 10:50:39 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int		manage_redirection(t_data *data, int i, int **fd, int last_read);
int		close_in_out_files(t_data *data);

void	exec_builtin_child(t_data *data, int i, int **fd, int last_read)
{
	int	exit_status;
	int	tab_size;

	tab_size = data[i].tab_size;
	manage_redirection(data, i, fd, last_read);
	// close all unused pipes
	if (i == 0)
	{
		close(fd[i][0]);
		for (int y = i + 1; y < tab_size - 1; y++)
		{
			close(fd[y][0]);
			close(fd[y][1]);
		}
	}
	else if (i < tab_size - 1)
	{
		close(fd[i][0]);
		for (int y = i + 1; y < tab_size - 1; y++)
		{
			close(fd[y][0]);
			close(fd[y][1]);
		}
	}
	exit_status = exec_builtin(data, i, fd, last_read);
	// close used pipes
	if (i == 0)
		close(fd[i][1]);
	else if (i < tab_size - 1)
	{
		close(last_read);
		close(fd[i][1]);
	}
	else
		close(last_read);
	while (i < tab_size)
	{
		close_in_out_files(&data[i]);
		i++;
	}
	free_pipes(fd, data->tab_size - 1);
	free_struct(data, tab_size);
	exit(exit_status);
}
