/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/29 12:59:12 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void  free_pipes(int **tab, int size);

// test :
// -  cat < file2.txt | rev > file1.txt
static int  ft_dup(int read_fd, int write_fd)
{
  if (read_fd != STDIN_FILENO)
    if (dup2(read_fd, STDIN_FILENO) == -1)
    {
      if (write_fd != STDOUT_FILENO)
        close(write_fd);
      return (close(read_fd), perror("dup read_fd "), -1);
    }
  if (write_fd != STDOUT_FILENO)
    if (dup2(write_fd, STDOUT_FILENO) == -1)
    {
      if (read_fd != STDIN_FILENO)
        close(read_fd);
      return (close(write_fd), perror("dup write_fd "), -1);
    }
  return(0);
}

int get_cmd_path(t_data *data)
{
	char	*directory;
  int   ret_value;

  if (!data->args.tab)
    return (1);
  directory = NULL;
  ret_value = check_all_dirs(data, &directory); // fill directory with the path where data->arg.tab[0] is located
  if (ret_value)
    return (ret_value);
  if (!directory)
    return (ft_perror("command not found\n"), 1);
  data->cmd_path = ft_concat_path(directory, data->args.tab[0]); // concate directory with args.tab[0] for the complete path of the commande (ex : /usr/bin/cat)
  free(directory);
  if (!data->cmd_path)
    return (-1);
  return (0);
}

int pipe_management(t_data data, int i, int tab_size, int **fds, int last_read)
{
  int index = 0;
  int status = -1;
  printf("IN_OUT : IN == %d OUT == %d\n", data.in_out_fd[0], data.in_out_fd[1]);

  if (data.input.size && data.output.size)
  {
    if (ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
        return (close_fds(fds, tab_size - 1, data.in_out_fd), -1);
    // close pipe
    if (i == tab_size - 1)
      return (close(last_read));
  }
  // cat <infile >a | cat >outfile
  else if (i == 0)
  {
    if (!data.output.size)
    {
      if (ft_dup(data.in_out_fd[0], fds[i][1]) == -1)
          return (close_fds(fds, tab_size - 1, data.in_out_fd), -1);
    }
    else
    {
      if (ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
          return (close_fds(fds, tab_size - 1, data.in_out_fd), -1);
    }
  }
  // out == data.in_out_fd[1]
  // IN == data.in_out_fd[0] -> redir file
  // IN == fd[i][0] -> redir pipe
  else if (i == tab_size - 1)
  {
    if (!data.input.size)
    {
      if (ft_dup(last_read, data.in_out_fd[1]) == -1)
          return (close_fds(fds, tab_size -1, data.in_out_fd), -1);
    }
    else
    {
      if (ft_dup(data.in_out_fd[0], data.in_out_fd[1]) == -1)
        return (close(last_read), -1);
    }
    close(last_read);
    close_fds(NULL, 0, data.in_out_fd);
    return (0);
  }
  close_fds(fds, tab_size - 1, data.in_out_fd);
  free_pipes(fds, tab_size - 1);
  return (0);
}

static int	exec_all(t_data *data, int tab_size, int **fd)
{
  int   i;
	pid_t	pid;
  int   ret_value;
  int   last_read_fd;

  last_read_fd = 0;
  i = 0;
  while (i < tab_size)
	{
    ret_value = init_structure(&(data[i]));
    if (ret_value)
      return (ret_value);
    ret_value = get_cmd_path(&(data[i]));
    if (ret_value)
      return (ret_value);

    pid = fork();
    if (pid < 0)
      return (perror("Fork failed "), pid);  // -1 -> crash

    if (pid == 0)
    {
      ret_value = pipe_management(data[i], i, tab_size, fd, last_read_fd);
      if (ret_value)
      {
        printf("error here i ==%d\n", i);
        exit(EXIT_FAILURE);
      }
      // if (is_builtin(&(data[i])))
      //   exec_builtin(&(data[i]), fd, tab_size);
      // else
      if (execve(data[i].cmd_path, data[i].args.tab, data[i].env.tab) == -1)
        exit(EXIT_FAILURE);
    }
    else
    {
      if (i == 0)
      {
        last_read_fd = fd[i][0];
        close(fd[0][1]);
        if (data[i].in_out_fd[0] != STDIN_FILENO)
          close(data[i].in_out_fd[0]);
        if (data[i].in_out_fd[1] != STDOUT_FILENO)
          close(data[i].in_out_fd[1]);
        // printf("THIS ONE\n");
        // close_fds(NULL, 0, data[i].in_out_fd);
      }
      else if (i == tab_size - 1)
      {
        printf("ICI\n");
        close(last_read_fd);
        if (data[i].in_out_fd[0] != STDIN_FILENO)
          close(data[i].in_out_fd[0]);
        if (data[i].in_out_fd[1] != STDOUT_FILENO)
          close(data[i].in_out_fd[1]);
      }
    }
    i++;
  }
  // for (int i = 0; i < tab_size - 1; i++){
  //   close_fds(NULL, 0, data[i].in_out_fd);
  // }
  free_pipes(fd, tab_size - 1);
  if (waitpid(pid, &(data[0].exit_status), 0) == -1)
    return (ft_perror("crash -> waitpid\n"), -1);
  return (0);
}

int exec_one(t_data *data)
{
  pid_t pid;
  int   ret_value;

  ret_value = init_structure(data);
  if (ret_value)
  {
    printf("ICI\n");
    return (ret_value); // -1 -> error : 1 -> back to prompt
  }
  ret_value = get_cmd_path(data);
  if (ret_value)
    return (ret_value); // -1 -> error : 1 -> back to prompt
  pid = fork();
  if (pid < 0)
    return (perror("Fork failed "), -1);
  if (pid > 0)
  {
    if (waitpid(pid, &(data->exit_status), 0) == -1)
      return (ft_perror("crash -> waitpid\n"), -1);
    return (0);
  }
  if (ft_dup(data->in_out_fd[0], data->in_out_fd[1]) == -1)
    return (close_fds(NULL, 0, data->in_out_fd), exit(EXIT_FAILURE), -1);
  // if (is_builtin(data))
  //   return (exec_builtin(data), 0);
  execve(data->cmd_path, data->args.tab, data->env.tab);
  close_fds(NULL, 0, data->in_out_fd);
  return (exit(EXIT_FAILURE), -1);
}

/*
  * before loop
    -> play heredocs
    -> open pipes

  * in loop on all instances of t_data *data in forks ?
    -> handle redirection
      . create all outputs
      . fill fds[1] with the new output fd
      . open inputs
      . fill fd[0] with the new input fd
    -> end of parsing
      - expand variables
      - clean args tokens

    -> if (is_builtins() == 1)
      - play builtin

    -> else
      - fork and execve();
*/
int	exec(int tab_size, t_data *data)
{
	int **pipe_fd;
  int ret_value = 0;

  pipe_fd = NULL;
  if (heredoc_management(data, tab_size) == -1)
    return(-1);
  if (tab_size == 1)
  {
    ret_value = exec_one(&(data[0]));
    if (close_fds(NULL, 0, data[0].in_out_fd) == -1)
      return (-1);
  }
  else
  {
    pipe_fd = init_pipe(tab_size - 1);
    if (!pipe_fd)
      return (-1);
    ret_value = exec_all(data, tab_size, pipe_fd);
    printf("PARENT RETURN TO MAIN\n");
    // for (int i = 0; i < tab_size; i++){
    //   if (close_fds(NULL, 0, data[i].in_out_fd) == -1)
    //     ret_value = -1;
    // }
  }
  return (ret_value);
}
