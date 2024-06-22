/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/22 19:00:11 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// test :
// -  cat < file2.txt | rev > file1.txt

static void	handle_parent(int i, int *fds, int prev_fd)
{
	if (i > 0)
		close(prev_fd);
	close(fds[1]);
}

int ft_dup(int infile_fd, int outfile_fd)
{
  if (infile_fd != STDIN_FILENO)
    if (dup2(infile_fd, STDIN_FILENO) == -1)
      return (close(infile_fd), -1);
  if (outfile_fd != STDOUT_FILENO)
    if (dup2(outfile_fd, STDOUT_FILENO) == -1)
      return (close(outfile_fd), -1);
  return (0);
}

static int	exec_handler(t_data *data, int *fds)
{
	char	*directory;
  int   ret_value;

	// if (!data || !data->args.tab)
	// 	return (-1);
	// if (is_builtin(data))
	// {
	// 	// if (redir_output(data, NULL) == -1)
	// 	// 	return (-1);
	// 	exec_builtin(data);
	// 	return (0);
	// }
	// else
	// {
  printf("EXEC HANDLER\n");
  if (ft_dup(fds[0], fds[1]) == -1)
    return (-1);
  ret_value = check_all_dirs(data, &directory); // fill directory with the path where data->arg.tab[0] is located
  if (ret_value == -1)
    return (-1);
  if (ret_value == 1)
    return (free(directory), 1);
  printf("directory path == %s\n", directory);
  data->cmd_path = ft_concat_path(directory, data->args.tab[0]); // concate directory with args.tab[0] for the complete path of the commande (ex : /usr/bin/cat)
  free(directory);
  if (!data->cmd_path)
    return (-1);
  printf("complete path == %s\n", data->cmd_path);

  if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
    return (perror("execve failed"), -1);
  return (0);
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

bool  init_fds(int **fds)
{
  *fds = malloc(2 * sizeof(int));
  if (!*fds)
    return (1);
  fds[0][0] = 0;
  fds[0][1] = 1;
  return (0);
}

void  close_free_fds(int *fds)
{
  if (fds[0] > 2)
    close(fds[0]);
  if (fds[1] > 2)
    close(fds[1]);
  free(fds);
}

/*
  * before loop
    -> preapare pipes ??
  * in loop on all instances of the t_data *data
    -> prepare the structure for exec :
      - expand variables
      - clean tokens
      - play inputs files routine
        . create all outputs
        . read inputs
        . play heredoc

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
	pid_t	pid;
  int   status;

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
    printf("fd[0] == %d, fd[1] == %d\n", fds[0], fds[1]);

    //if (tab_size > 1 && !data->output.size)
    // if (pipe(fds) == -1)
    //   return (perror("Pipe failed"), -1);

    pid = fork();
		if (pid == -1)
			return (perror("Fork failed "), close_free_fds(fds), -1);
		if (pid == 0)
		{
      if (exec_handler(data, fds) == -1)
      {
        printf("EXIT_FAILUR\n");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
		}
		if (pid > 0)
		{
      waitpid(pid, NULL, 0);
      if (fds[0] != 0)
      {
      //printf("return status waitpid == %d\n", status);

        close(fds[0]);
        fds[0] = 0;
      }
      if (fds[1] > 2)
      {
        close(fds[1]);
        fds[1] = 1;
      }
      if (fds[1] == 2)
        fds[1] = 1;
		}
		i++;
	}
	return (close_free_fds(fds), 0);
}
