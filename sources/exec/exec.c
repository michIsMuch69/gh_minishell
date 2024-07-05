/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/05 09:22:51 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


static int parent_routine(t_data *data, int i, int tab_size, int **fd)
{
    int ret_value;

    if (i == 0)
        ret_value = close(fd[i][1]);
    else
    {
        ret_value = close(fd[i - 1][0]);
        if (i < tab_size - 1)
            ret_value = close(fd[i][1]);
    }
    if (ret_value)
        perror("close pipe in parent ");
    ret_value = close_fds(data[i].in_out_fd);
    if (ret_value == -1)
        perror("close in_out files in parent ");
    return (ret_value);
}

static int child_routine(t_data *data, int tab_size, int i, int **fd)
{
    int ret_value;

    if (i < tab_size - 1)
    {
        if (i)
            ret_value = exec_redirection(data[i], fd[i], fd[i - 1][0]);
        else
            ret_value = exec_redirection(data[i], fd[i], 0);
    }
    else
        ret_value = exec_redirection(data[i], NULL, fd[i - 1][0]);
    if (i == tab_size -1)
    {
        if (close(fd[i - 1][0]) == -1)
            return (-1);
    }
    else
    {
        if (i && close_pipes(fd, (tab_size - 1), i, fd[i -1][0]) == -1)
                return (-1);
        else if (!i && close_pipes(fd, (tab_size - 1), i, 0) == -1)
            return (-1);
    }
    return (ret_value);
}

static int	exec_all(t_data *data, int tab_size, int **fd)
{
    int     i;
	pid_t   pid;
    int     ret_value;

    i = -1;
    while (++i < tab_size)
	{
        pid = fork();
        if (pid < 0)
            return (perror("Fork failed "), pid);  // -1 -> crash
        if (pid == 0)
        {
            if (child_routine(data, tab_size, i, fd) == -1)
                return (free_pipes(fd, tab_size -1), exit(1), 1);
            free_pipes(fd, tab_size - 1);
            if (execve(data[i].cmd_path, data[i].args.tab, data[i].env.tab) == -1)
                exit(EXIT_FAILURE);
        }
        else if (pid > 0)
            if (parent_routine(data, i, tab_size, fd) == -1)
                return (free_pipes(fd, tab_size - 1), -1);
    }
    free_pipes(fd, tab_size - 1);
    return (wait_all(data, tab_size));
}

static int exec_one(t_data *data)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        return (perror("Fork failed "), close_fds(data->in_out_fd), -1);
    if (pid > 0)
    {
        if (wait(&(data->exit_status)) == -1)
            return (ft_perror("crash -> wait()\n"), \
                    close_fds(data->in_out_fd), -1);
        return (close_fds(data->in_out_fd));
    }
    if (ft_dup(data->in_out_fd[0], data->in_out_fd[1]) == -1)
    {
        close_fds(data->in_out_fd);
        exit(EXIT_FAILURE);
    }
    // if (is_builtin(data))
    //   exec_builtin(data, 1, NULL);
    if (close_fds(data->in_out_fd) == -1)
        exit(EXIT_FAILURE);
    if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
        perror("execve ");
    exit(EXIT_FAILURE);
    return (-1);
}

/*
  * before loop
    -> play heredocs
    -> open pipes
    -> handle redirection
      . create all outputs
      . fill fds[1] with the new output fd
      . open inputs
      . fill fd[0] with the new input fd
    -> end of parsing
      - expand variables
      - clean args tokens

  * in loop on all instances of data
    -> if (is_builtins() == 1)
      - play builtin
    -> else
      - fork and execve();
*/
int	exec(int tab_size, t_data *data)
{
    int ret_value;
    int **pipe_fd;
    int i;

    i = -1;
    pipe_fd = NULL;
    ret_value = init_exec(data, tab_size);
    if (ret_value)
    {
        while (++i < tab_size)
            close_fds(data[i].in_out_fd);
        return (ret_value); // -1 -> crash : 1 -> back to prompt CLOSE ALL IN_OUT
    }
    if (tab_size == 1)
        ret_value = exec_one(&(data[0]));
    else
    {
        pipe_fd = init_pipe(data, tab_size - 1);
        if (!pipe_fd)
            return (-1); // CLOSE ALL IN_OUT
        ret_value = exec_all(data, tab_size, pipe_fd);
    }
    return (ret_value);
}
