/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:38:31 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/05 09:20:35 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int  ft_dup(int read_fd, int write_fd)
{
  if (read_fd > STDIN_FILENO)
    if (dup2(read_fd, STDIN_FILENO) == -1)
    {
      if (write_fd > STDOUT_FILENO)
        close(write_fd);
      return (close(read_fd), perror("dup read_fd "), -1);
    }
  if (write_fd > STDOUT_FILENO)
    if (dup2(write_fd, STDOUT_FILENO) == -1)
    {
      if (read_fd > STDIN_FILENO)
        close(read_fd);
      return (close(write_fd), perror("dup write_fd "), -1);
    }
  return(0);
}

static int fork_redir(t_data data, int *fds, int last_read)
{
    if (fds != NULL)
    {
        if (!data.input.size && !data.output.size && \
            ft_dup(last_read, fds[1]) == -1)
            return (-1);
        else if (data.input.size && !data.output.size && \
            ft_dup(data.in_out_fd[0], fds[1]) == -1)
            return (-1);
        else if (!data.input.size && data.output.size && \
            ft_dup(last_read, data.in_out_fd[1]) == -1)
            return (-1);
    }
    else
    {
        if (!data.input.size && ft_dup(last_read, data.in_out_fd[1]) == -1)
            return (-1);
        else if (data.input.size && \
            ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
            return (-1);
    }
    return (0);
}

int exec_redirection(t_data data, int *fds, int last_read)
{
    if (data.input.size && data.output.size)
    {
        if (ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
            return (-1);
    }
    if (!last_read)
    {
        if (!data.output.size)
        {
            if (ft_dup(data.in_out_fd[0], fds[1]) == -1)
                return (-1);
        }
        else
        {
            if (ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
                return (-1);
        }
    }
    return (fork_redir(data, fds, last_read));
}

int close_fds(int *in_out_fd)
{
	int status;

	status = 0;
    if (in_out_fd[0] > STDIN_FILENO)
        if (close(in_out_fd[0]) == -1)
            status = -1;
    if (in_out_fd[1] > STDOUT_FILENO)
        if (close(in_out_fd[1]) == -1)
            status = -1;
    in_out_fd[0] = -1;
    in_out_fd[1] = -1;
	if (status == -1)
		perror("close_fds :");
	return (status);
}
