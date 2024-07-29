/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:39:26 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 11:32:49 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "includes.h"
#include "builtins.h"

int	exec_builtin(t_data *data, int i, int **fds, int last_fd)
{
	int	status;

	status = 0;
	if (ft_strcmp(data[i].args.tab[0], "pwd") == 0)
		data[0].exit_status = ft_pwd();
	else if (ft_strcmp(data[i].args.tab[0], "echo") == 0)
		data[0].exit_status = ft_echo(&data[i]);
	else if (ft_strcmp(data[i].args.tab[0], "env") == 0)
		data[0].exit_status = ft_env(&data[i]);
	else if (ft_strcmp(data[i].args.tab[0], "cd") == 0)
		data[0].exit_status = ft_cd(&data[i]);
	else if (ft_strcmp(data[i].args.tab[0], "exit") == 0)
		data[0].exit_status = ft_exit(data, i, fds, last_fd);
	else if (ft_strcmp(data[i].args.tab[0], "export") == 0)
		data[0].exit_status = ft_export(&data[i], &data[i].export);
	else if (ft_strcmp(data[i].args.tab[0], "unset") == 0)
		data[0].exit_status = ft_unset(&data[i]);
	return (data[0].exit_status);
}
