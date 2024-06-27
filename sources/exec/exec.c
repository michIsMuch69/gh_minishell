/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/26 20:01:19 by florian          ###   ########.fr       */
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
      return (close(read_fd), perror("dup read_fd "), -1);
  if (write_fd != STDOUT_FILENO)
    if (dup2(write_fd, STDOUT_FILENO) == -1)
      return (close(write_fd), perror("dup write_fd "), -1);
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

static int	exec_all(t_data *data, int tab_size, int **fd, int *status)
{
  int   i;
	pid_t	pid;

  i = 0;
  while (i < tab_size)
	{
    *status = init_structure(&(data[i]));
    if (*status)
      return (*status);
    *status = get_cmd_path(data);
      if (*status)
        return (*status);
    *status = pipe_management(data->in_out_fd, fd[i]);
    pid = fork();
    if (pid < 0)
      return (perror("Fork failed "), pid);  // -1 -> crash
    if (pid > 0)
      ;
    else
    {
      if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
        printf("EXIT_FAILUR\n");
      return (exit(EXIT_FAILURE), 1);
    }
    i++;
  }
  if (waitpid(-1, status, 0) == -1)
    return (-1);
  return (pid);
}

int exec_one(t_data *data, int *status)
{
  pid_t pid;

  *status = init_structure(data);
  if (*status)
    return (*status); // -1 -> error : 1 -> back to prompt
  *status = get_cmd_path(data);
  if (*status)
    return (*status); // -1 -> error : 1 -> back to prompt
  // if(is_builtin(data))
  //   -> return (play builtins(data, status));
  pid = fork();
  if (pid < 0)
    return (perror("Fork failed "), -1);
  if (pid > 0)
  {
    if (waitpid(pid, status, 0) == -1)
      return (-1);
    return (close_fds(NULL, 0, data->in_out_fd));
  }
  if (ft_dup(data->in_out_fd[0], data->in_out_fd[1]) == -1)
    return (close_fds(NULL, 0, data->in_out_fd), exit(EXIT_FAILURE), -1);
  if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
    return (close_fds(NULL, 0, data->in_out_fd), exit(EXIT_FAILURE), -1);
  return (-1);
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
	int		**pipe_fd;
  int   status;
  pid_t pid;

  if (heredoc_management(data, tab_size) == -1)
    return(-1);
  if (tab_size == 1)
  {
    pid = exec_one(&(data[0]), &status);
    if (pid == -1 || pid == 1)
      return (close_fds(NULL, 0, data->in_out_fd), pid); // -1 -> crash : 1 -> back to prompt
    // all fds are close, have to manage the return value;
  }
  else
  {
    pipe_fd = init_pipe(tab_size - 1);
    if (!pipe_fd)
      return (-1);
    pid = exec_all(data, tab_size, pipe_fd, &status);
    if (pid == -1 || pid == 1)
      return (close_fds(pipe_fd, tab_size - 1, data->in_out_fd), pid);
    // manage return value;
    status = close_fds(pipe_fd, tab_size -1, NULL);
  }
  // have to manage the return value of the command played
    // fill the $? variable
  return (status);
}
