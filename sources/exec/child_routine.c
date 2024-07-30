/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:21:37 by florian           #+#    #+#             */
/*   Updated: 2024/07/30 07:42:21 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exec_builtin_child(t_data *data, int i, int **fd, int last_read);
int		close_in_out_files(t_data *data);

int	ft_dup(int read_fd, int write_fd)
{
	if (read_fd > STDIN_FILENO)
	{
		if (dup2(read_fd, STDIN_FILENO) == -1)
		{
			if (write_fd > STDOUT_FILENO)
				close(write_fd);
			return (close(read_fd), perror("dup read_fd "), -1);
		}
	}
	if (write_fd > STDOUT_FILENO)
	{
		if (dup2(write_fd, STDOUT_FILENO) == -1)
		{
			if (read_fd > STDIN_FILENO)
				close(read_fd);
			return (close(write_fd), perror("dup write_fd "), -1);
		}
	}
	return (0);
}

int	manage_redirection(t_data *data, int i, int **fd, int last_read)
{
	int	ret_value;

	if (i == 0)
	{
		if (data[i].output.size > 0)
			ft_dup(data[i].in_out_fd[0], data[i].in_out_fd[1]);
		else
			ft_dup(data[i].in_out_fd[0], fd[i][1]);
	}
	else if (i < data[i].tab_size - 1)
	{
		if (data[i].input.size > 0 && data[i].output.size > 0)
			ft_dup(data[i].in_out_fd[0], data[i].in_out_fd[1]);
		else if (data[i].input.size > 0)
			ft_dup(data[i].in_out_fd[0], fd[i][1]);
		else if (data[i].output.size > 0)
			ft_dup(last_read, data[i].in_out_fd[1]);
		else
			ft_dup(last_read, fd[i][1]);
	}
	else
	{
		if (data[i].input.size > 0)
			ft_dup(data[i].in_out_fd[0], data[i].in_out_fd[1]);
		else
			ft_dup(last_read, data[i].in_out_fd[1]);
	}
	return (0);
}

int	child_routine(t_data *data, int i, int **fd, int last_read)
{
	int	tab_size;

	tab_size = data[i].tab_size;
	if (is_builtin(&data[i]))
		exec_builtin_child(data, i, fd, last_read);
	manage_redirection(data, i, fd, last_read);
		// all redir are made no close no free
	close_fds(fd, data->tab_size - 1, i, last_read);
	while (i < tab_size)
	{
		close_in_out_files(&data[i]);
		i++;
	}
	free_pipes(fd, data->tab_size - 1);
	return (0);
}
