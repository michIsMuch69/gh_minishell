/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 18:29:46 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


static int	exec_handler(t_data *data)
{
	char	*directory;
	char	*cmd_path;

	if (!data || !data->args.tab)
		return (-1);
	if (is_builtin(data))
	{
		if (redir_output(data, NULL) == -1)
			return (-1);
		exec_builtin(data);
		return (0);
	}
	else
	{
		directory = check_all_dirs(data->args.tab[0]);
		if (!directory)
			return (perror("Command not found"), -1);
		cmd_path = ft_concat_path(directory, data->args.tab[0]);
		if (!cmd_path)
			return (free(directory), -1);
    free(directory);
		if (execve(cmd_path, data->args.tab, data->env.tab) == -1)
			return (perror("execve failed"), free(cmd_path), -1);
	}
	return (0);
}

// test :
// -  cat < file2.txt | rev > file1.txt

static void	handle_parent(int i, int *fds, int prev_fd)
{
	if (i > 0)
		close(prev_fd);
	close(fds[1]);
}


static int  set_structure(t_data *data, int fds[2], int prev_fd)
{
	int		ret_value;

  ret_value = clean_struct(data);
  if (ret_value == -1)
    return (-1); // error exit term
  if (ret_value == 1)
    return (1); // back to prompt
  ret_value = handle_redirection(fds, prev_fd, data);
  if (ret_value == -1)
    return (-1); // error exit term
  if (ret_value == 1)
    return (1); // back to prompt
  return (0);
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
	int		i;
	int		prev_fd;
	int		fds[2];
	pid_t	pid;

  int   ret_value;


	prev_fd = -1;
	i = 0;

	while (i < tab_size)
	{
    ret_value = set_structure(&(data[i]), fds, prev_fd);
    if (ret_value == -1)
      return (-1); // close minishell
    if (ret_value == 1)
      return (1); // back to prompt

    //if (!data->output.size)
    if (pipe(fds) == -1)
      return (perror("Pipe failed"), -1);

    pid = fork();
		if (pid == -1)
			return (perror("Fork failed"), close(fds[0]), close(fds[1]), -1);
		else if (pid == 0)
		{
      if (exec_handler(data) == -1)
        return (close(fds[0]), close(fds[1]), -1);
      close(fds[0]);
      close(fds[1]);
		}
		else
		{
      //waitpid(pid, NULL, 0);
			handle_parent(i, fds, prev_fd);
			prev_fd = fds[0];
		}
		i++;
	}
	wait_all(tab_size);
	return (0);
}
