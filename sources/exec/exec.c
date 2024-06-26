/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/26 10:56:50 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

bool  init_pipe(int ***pipe_tab, int size);
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

static int	exec_handler(t_data *data, int *in_out)
{
	pid_t	pid;
  int   ret_value;

  ret_value = get_cmd_path(data);
  if (ret_value == -1 || ret_value == 1)
    return (ret_value); // -1 -> crash : 1 -> didn't found exec //
  // if (is_builtin(data))
    // play_builtin();
  pid = fork();
  if (pid < 0)
    return (perror("Fork failed "), -1);
  if (pid > 0)
    return (pid);
  if (ft_dup(in_out[0], in_out[1]) == -1)
    exit(EXIT_FAILURE);
  if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
    printf("EXIT_FAILUR\n");
  return (exit(EXIT_FAILURE), 1);
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
	int		i;

  if (heredoc_management(data, tab_size) == -1)
    return(-1);
  if (tab_size > 1)
    if (init_pipe(&pipe_fd, tab_size - 1))
      return (-1);
  i = 0;
	while (i < tab_size)
	{
    if (tab_size == 1)
    {
      status = init_structure(&(data[i]), data[i].in_out_fd);
      if (status)
        return (status);
      status = exec_handler(&(data[i]), data[i].in_out_fd);
      if (status > 1) // status > 1 == child pid
        waitpid(status, NULL, 0);
      close_fds(NULL, 0, data->in_out_fd);
      if (status <= 1)
        return (status);  // -1 -> crash : 1 -> back to prompt
      i++;
    }
    // else
    // {
    //     go_to pipe
    // }


    //status = exec_handler(&(data[i]), fds, is_pipe); // here, status get child_pid
    // if (status == -1 || status == 1)
    //   return (status); // -1 -> crash : 1 -> cannot found exec
    // if (is_pipe && close(fds[3]) == -1)
    //   return (perror("close in parent "), close_free_fds(fds), -1);
    // if (is_pipe)
    // {
    //   if (fds[0] != STDIN_FILENO)
    //     close(fds[0]);
    //   if (fds[1] != STDOUT_FILENO)
    //     close(fds[1]);
    //   fds[0] = fds[2];
    //   fds[1] = STDOUT_FILENO;
    // }
    // else
    // {
    //   if (fds[0] != STDIN_FILENO)
    //     close(fds[0]);
    //   if (fds[1] != STDOUT_FILENO)
    //     close(fds[1]);
    //   fds[0] = STDIN_FILENO;
    //   fds[1] = STDOUT_FILENO;
    // }
  }
	return (0);
}
