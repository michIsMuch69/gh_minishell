/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/10 11:30:10 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin_parent(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "exit") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "export") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "unset") == 0)
		return (1);
	return (0);
}

int	is_builtin_child(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "echo") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "env") == 0)
		return (1);
	return (0);
}

/*
  ###### TEST A FAIRE ######
  -> avant de exit(status)
	- il faut close les fd de data : free_fds(NULL, 0, data->in_out);
	- il faut free pipe_ptr avant de exit(status); free_pipes(pipe_ptr, (tab_size - 1))
*/

void	exec_builtin_parent(t_data *data, int tab_size, int i, int **fd)
{
	int	status;

	status = 0;
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
	{
		printf("Executing built-in cd in parent process\n");

		status = ft_cd(data->args.tab, data->env.tab);
		if (status == -1)
			perror("cd failed");
		close_fds(data->in_out_fd);
		return ;
		// cd doesn't exit the shell, so no exit(status) at the end.
	}
	else if (ft_strcmp(data->args.tab[0], "exit") == 0)
	{
		printf("Executing built-in exit in parent process\n");

		ft_exit(data->args.tab, data->exit_status);
	}
	//printf("Unexpected exit from exec_builtin_parent\n");
	// else if (ft_strcmp(data->args.tab[0], "export") == 0)
	// {
	//     status = ft_export(data->args.tab);
	//     return;  // cd doesn't exit the shell, so no exit(status) at the end.
	// }
	else if (ft_strcmp(data->args.tab[0], "unset") == 0)
	{
	    status = ft_unset(data->args.tab[1], &(data->env));
	    return;  // cd doesn't exit the shell, so no exit(status) at the end.
	}
	// only exit the shell if the exit command is called
	exit(status);
}

void	exec_builtin_child(t_data *data, int tab_size, int i, int **fd)
{
	int	status;

	status = 0;
	if (ft_strcmp(data->args.tab[0], "pwd") == 0)
	{
		status = ft_pwd();
	}
	else if (ft_strcmp(data->args.tab[0], "echo") == 0)
	{
	    status = ft_echo(data->args.tab);
	}
	else if (ft_strcmp(data->args.tab[0], "env") == 0)
	{
	    status = ft_env(data->env.tab);
	}
	status = close_pipes(fd, tab_size - 1, i, 0);
	while (i < tab_size)
	{
		close_fds(data[i].in_out_fd);
		i++;
	}
	free_pipes(fd, tab_size - 1);
	free_struct(data, tab_size);
	exit(status);
}
