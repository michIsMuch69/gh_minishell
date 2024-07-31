/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 15:41:24 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int			exec_one(t_data *data);
int			child_routine(t_data *data, int i, int **fd, int last_fd);
int			close_in_out_files(t_data *data);

int	close_in_out_files(t_data *data)
{
	if (data->input.size > 0)
	{
		if (data->in_out_fd[0] > STDIN_FILENO)
			if (close(data->in_out_fd[0]) == -1)
				perror("close input file ");
	}
	if (data->output.size > 0)
	{
		if (data->in_out_fd[1] > STDOUT_FILENO)
			if (close(data->in_out_fd[1]) == -1)
				perror("close output file ");
	}
	return (0);
}

static int	parent_routine(t_data *data, int i, int **fd, int *last_read)
{
	if (i == 0)
	{
		if (close(fd[i][1]) == -1)
			perror("close in parent i == 0 ");
		*last_read = fd[i][0];
		close_in_out_files(&data[i]);
	}
	else if (i < data[i].tab_size - 1)
	{
		if (close(*last_read) == -1)
			perror("close in parent i < tab_size - 1 last read");
		if (close(fd[i][1]) == -1)
			perror("close in parent i < tab_size - 1 write pipe");
		*last_read = fd[i][0];
		close_in_out_files(&data[i]);
	}
	else
	{
		if (close(*last_read) == -1)
			perror("close in parent i == tab_size - 1 last read");
		close_in_out_files(&data[i]);
	}
	return (0);
}

static int	exec_all(t_data *data, int tab_size, int **fd)
{
	pid_t	pid;
	int		last_read;
	int		i;

	i = -1;
	last_read = 0;
	while (++i < data->tab_size)
	{
		pid = fork();
		if (pid < 0)
			return (perror("Fork failed "), pid);
		if (pid == 0)
		{
			if (child_routine(data, i, fd, last_read) == -1)
				return (1);
			if (execve(data[i].cmd_path, data[i].args.tab, data[i].env.tab) == -1)
				exit(EXIT_FAILURE);
		}
		else if (pid > 0)
			if (parent_routine(data, i, fd, &last_read) == -1)
				return (1);
	}
	return (wait_all(data, tab_size, pid, fd));
}

void	exec(int tab_size, t_data *data)
{
	int	**pipe_fd;
	int	i;

	i = 0;
	data[0].exit_status = init_exec(data, tab_size);
	if (data[0].exit_status)
	{
		while (i < tab_size)
		{
			close_in_out_files(&data[i]);
			i++;
		}
	}
	else if (tab_size == 1)
		exec_one(&data[0]);
	else
	{
		pipe_fd = init_pipe(data, tab_size - 1);
		if (!pipe_fd)
			return ;
		exec_all(data, tab_size, pipe_fd);
	}
}
