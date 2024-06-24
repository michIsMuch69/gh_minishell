/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/24 12:36:32 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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

  if (!data->args.tab[0])
    return (1);
  ret_value = check_all_dirs(data, &directory); // fill directory with the path where data->arg.tab[0] is located
  if (ret_value == -1)
    return (-1);
  if (ret_value == 1)
    return (free(directory), 1);
  data->cmd_path = ft_concat_path(directory, data->args.tab[0]); // concate directory with args.tab[0] for the complete path of the commande (ex : /usr/bin/cat)
  free(directory);
  if (!data->cmd_path)
    return (-1);
  return (0);
}

static int	exec_handler(t_data *data, int *fds, bool is_pipe)
{
	pid_t	pid;
  int   ret_value;

  ret_value = get_cmd_path(data);
  if (ret_value == -1)
    return (-1); // crash
  if (ret_value == 1)
    return (1); // didn't found exec

  pid = fork();
  if (pid < 0)
    return (perror("Fork failed "), -1);
  if (pid > 0)
    return (pid);
  // IN CHILD FROM HERE //
  if (is_pipe)
  {
    // printf("IS PIPE\n");
    if (ft_dup(fds[0], fds[3]) == -1)
      exit(EXIT_FAILURE);
    if (close(fds[2]) == -1)
      exit(EXIT_FAILURE);
  }
  else
  {
    // printf("TOUR 2\n");
    // printf("fd[0] == %d, fd[1] == %d\nfd[2] == %d, fd[3] == %d\n", \
    //         fds[0], fds[1], fds[2], fds[3]);
    // printf("TOUR 2\n");
    if (ft_dup(fds[0], fds[1]) == -1)
      exit(EXIT_FAILURE);

  }
  if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
  {
    printf("EXIT_FAILUR\n");
    exit(EXIT_FAILURE);
  }
  return (0);

	// if (is_builtin(data))
	// {
	// 	// if (redir_output(data, NULL) == -1)
	// 	// 	return (-1);
	// 	exec_builtin(data);
	// 	return (0);
	// }
	// else
	// {
  // if (ft_dup(fds[0], fds[1]) == -1)
  //   return (-1);

}


static int  set_structure(t_data *data, int *fds)
{
	int ret_value;

  ret_value = clean_struct(data);
  if (ret_value == -1)
    return (-1); // error exit term
  if (ret_value == 1)
    return (1); // back to prompt
  if (data->input.size || data->output.size)
  {
    ret_value = handle_redirection(fds, data);
    if (ret_value == -1)
      return (-1); // error exit term
    if (ret_value == 1)
      return (1); // back to prompt
  }
  return (0);
}

int manage_pipe(t_data *data, int *fds)
{
  int pipe_fd[2];

  if (pipe(pipe_fd) == -1)
    return (perror("pipe init"), -1);
  fds[2] = pipe_fd[0];
  fds[3] = pipe_fd[1];
  return (0);
}

/*
  * before loop
    -> in init_fds()
      - init fds array (alloc tab + define fds values)

  * in loop on all instances of t_data *data

    -> in set_structure :
      - expand variables
      - clean tokens
      - manage output redirection
        . create all outputs
        . fill fds[1] with the new output fd
      - manage input redirection
        . play heredoc
        . open inputs
        . fill fd[0] with the new input fd
      - None of these functions redirect STDIN or STDOUT, NOT YET !

    -> if (is_builtins() == 1)
      - play builtin

    -> else
      - fork and execve();
*/
int	exec(int tab_size, t_data *data)
{
  int   ret_value;
	int		*fds;
	int		i;

	int		prev_fd = -1;
  int   status;
  bool is_pipe = 0;

	i = 0;
  if (init_fds(&fds))
    return (-1);


	while (i < tab_size)
	{
    ret_value = set_structure(&(data[i]), fds);
    if (ret_value == -1)
      return (-1); // close minishell all is free
    if (ret_value == 1)
      return (1); // back to prompt all is free


    if (!data[i].output.size && i < tab_size - 1)
    {
      if (manage_pipe(&(data[i]), fds) == -1)
        return (close_free_fds(fds), -1);
      is_pipe = 1;
    }
    // printf("fd[0] == %d, fd[1] == %d\nfd[2] == %d, fd[3] == %d\n", \
    //         fds[0], fds[1], fds[2], fds[3]);


    ret_value = exec_handler(&(data[i]), fds, is_pipe); // here, ret_value get child_pid
    if (ret_value == -1)
      return (close_free_fds(fds), -1); // crash
    if (ret_value == 1)
      return (close_free_fds(fds), 1); // cannot found exec
    if (is_pipe && close(fds[3]) == -1)
      return (perror("close in parent "), close_free_fds(fds), -1);
    waitpid(ret_value, NULL, 0);
    if (is_pipe)
    {
      if (fds[0] != STDIN_FILENO)
        close(fds[0]);
      if (fds[1] != STDOUT_FILENO)
        close(fds[1]);
      fds[0] = fds[2];
      fds[1] = STDOUT_FILENO;
    }
    is_pipe = 0;
		i++;
	}


	return (close_free_fds(fds), 0);
}
