/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:04:55 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/30 07:42:54 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	exec_child(t_data *data, int saved_std[])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork exec_one "), -1);
	if (pid == 0)
	{
		if (redir_file(data))
			exit(EXIT_FAILURE);
		if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
			exit(EXIT_FAILURE);
	}
	else
	{
		close_all_redir(data, saved_std);
		if (waitpid(pid, &(data[0].exit_status), 0) == -1)
			return (-1);
		return (0);
	}
	return (0);
}

static int	exec_built(t_data *data, int saved_std[])
{
	int	exit_stat;

	if (save_std_fileno(data, saved_std))
		return (1);
	if (redir_file(data))
		return (1);
	exit_stat = exec_builtin(data, 0, NULL, 0);
	if (reset_std_fileno(data, saved_std))
		return (close_all_redir(data, saved_std), 1);
	return (close_all_redir(data, saved_std), exit_stat);
}

int	exec_one(t_data *data)
{
	int	saved_std[2];

	saved_std[0] = STDIN_FILENO;
	saved_std[1] = STDOUT_FILENO;
	if (is_builtin(data))
		return (exec_built(data, saved_std));
	else
		return (exec_child(data, saved_std));
	return (-1);
}
