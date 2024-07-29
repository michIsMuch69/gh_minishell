/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:41:42 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 17:05:48 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include "exec.h"

int close_last_fd(int last_fd)
{
    if (last_fd > STDIN_FILENO)
    {
        if (close(last_fd) == -1)
        {
            perror("close_last_fd");
            return -1;
        }
    }
    return 0;
}

int close_write_end(int **fds, int size, int i_start)
{
    int status = 0;

    while (i_start < size)
    {
        if (fds != NULL && fds[i_start] != NULL)
        {
            if (fds[i_start][1] > STDOUT_FILENO && fds[i_start][1] != -1)
            {
                if (close(fds[i_start][1]) == -1)
                {
                    status = -1;
                    perror("close_write_ends - closing write end");
                }
                else
                    fds[i_start][1] = -1;
            }
        }
        i_start++;
    }

    return status;
}

int close_read_end(int **fds, int size, int i_start)
{
    int status = 0;

    while (i_start < size)
    {
        if (fds != NULL && fds[i_start] != NULL)
        {
            if (fds[i_start][0] > STDIN_FILENO && fds[i_start][0] != -1)
            {
                if (close(fds[i_start][0]) == -1)
                {
                    status = -1;
                    perror("close_read_ends - closing read end");
                }
                else
                    fds[i_start][0] = -1;
            }
        }
        i_start++;
    }

    return (status);
}

int close_pipe_fds(int **fds, int size, int i_start)
{
    int status = 0;

    if (close_write_end(fds, size, i_start) == -1)
        status = -1;
    if (close_read_end(fds, size, i_start) == -1)
        status = -1;
    return (status);
}

