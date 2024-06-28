/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/27 19:50:58 by florian          ###   ########.fr       */
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
      if (write_fd)
        close(write_fd);
      return (close(read_fd), perror("dup read_fd "), -1);
    }
  if (write_fd != STDOUT_FILENO)
    if (dup2(write_fd, STDOUT_FILENO) == -1)
    {
      if (read_fd)
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


int pipe_management(t_data data, int i, int tab_size, int **fds)
{
  if (i == 0)
  {
    // printf("LA PIPE IN == %d OUT == %d\n", fds[i][0], fds[i][1]);
    // printf("LA IN_OUT_FD IN == %d OUT == %d\n", data.in_out_fd[0], data.in_out_fd[1]);
    if (data.input.size && data.output.size)
    {
      if (close(fds[i][0]) == -1 || close(fds[i][1]) == -1)
        return (-1);
      return (0);
    }
    if (close(fds[i][0]) == -1)
      return (close(fds[i][1]), -1);
    if (data.input.size)
      if (dup2(data.in_out_fd[0], STDIN_FILENO) == -1)
        return (close(fds[i][1]), -1);

    if (!data.output.size)
      if (dup2(fds[i][1], STDOUT_FILENO) == -1)
        return (close(fds[i][1]), -1);
    else
      if (dup2(data.in_out_fd[1], STDOUT_FILENO) == -1)
        return (close(fds[i][1]), -1);
    // if (close(fds[i][1]))
    //   return (-1);
    return (0);
  }
  else if (i == tab_size - 1)
  {
    // printf("ICI IN == %d OUT == %d\n", fds[i][0], fds[i][1]);
    // printf("ICI IN_OUT_FD IN == %d OUT == %d\n", data.in_out_fd[0], data.in_out_fd[1]);
    i--;
    if (data.input.size && data.output.size)
    {
      if (close(fds[i][0]) == -1 || close(fds[i][1]) == -1)
        return (-1);
      return (0);
    }
    if (close(fds[i][1]) == -1)
      return (close(fds[i][0]), -1);

    if (data.output.size)
      if (dup2(data.in_out_fd[1], STDOUT_FILENO) == -1)
        return (close(fds[i][0]), -1);

    if (!data.input.size)
      if (dup2(fds[i][0], STDIN_FILENO) == -1)
        return (close(fds[i][0]), -1);
    else
      if (dup2(data.in_out_fd[0], STDIN_FILENO) == -1)
        return (close(fds[i][0]), -1);
    // if (close(fds[i][0]))
    //   return (-1);
    return (0);

















    if (close(fds[i][1]))
      return (-1);

    if (data.in_out_fd[1] != STDOUT_FILENO)
      dup2(data.in_out_fd[1], STDOUT_FILENO);

    if (data.in_out_fd[0] == STDIN_FILENO)
      dup2(fds[i][0], STDIN_FILENO);

    if (close(fds[i][0]))
      return (-1);
    }
    // printf("\n");
  return (0);
}

static int	exec_all(t_data *data, int tab_size, int **fd)
{
  int   i;
	pid_t	pid;
  int   ret_value;

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
      ret_value = pipe_management(data[i], i, tab_size, fd);
      if (ret_value)
        exit(EXIT_FAILURE);
      // if (is_builtin(&(data[i])))
      //   exec_builtin(&(data[i]), fd, tab_size);
      // else
      if (execve(data[i].cmd_path, data[i].args.tab, data[i].env.tab) == -1)
        exit(EXIT_FAILURE);
      return (exit(EXIT_FAILURE), -1);
    }
    else
    {
      // if (i < tab_size - 1)
      // {
      //   close(fd[i][0]);
      //   close(fd[i][1]);
      // }
      //close_fds(NULL, 0, data[i].in_out_fd);
      i++;
    }
  }
  close_fds(fd, tab_size - 1, NULL);
  if (waitpid(pid, &(data[0].exit_status), 0) == -1)
    return (ft_perror("crash -> waitpid\n"), -1);
  for (int y = 0; y < tab_size; y++)
  {
    if (close_fds(NULL, 0, data[y].in_out_fd) == -1)
      return (-1);
  }
  printf("WAIT OVER status == %d\n", data[0].exit_status);
  return (0);
}

int exec_one(t_data *data)
{
  pid_t pid;
  int   ret_value;

  ret_value = init_structure(data);
  if (ret_value)
    return (ret_value); // -1 -> error : 1 -> back to prompt
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
  int ret_value;

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
    for (int i = 0; i < tab_size; i++){
      if (close_fds(NULL, 0, data[i].in_out_fd) == -1)
        ret_value = -1;
    }
  }
  return (ret_value);
}
