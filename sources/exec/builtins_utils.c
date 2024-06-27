/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/27 12:13:45 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(t_data *data)
{
	// if (ft_strcmp(data->args.tab[0], "echo") == 0)
	// 	return (1);
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
		return (1);
	// if (ft_strcmp(data->args.tab[0], "pwd") == 0)
	// 	return (1);
	// if (ft_strcmp(data->args.tab[0], "export") == 0)
	// 	return (1);
	// if (ft_strcmp(data->args.tab[0], "unset") == 0)
	// 	return (1);
	// if (ft_strcmp(data->args.tab[0], "env") == 0)
	// 	return (1);
	if (ft_strcmp(data->args.tab[0], "exit") == 0)
		return (1);
	return (0);
}

/*
  ###### TEST A FAIRE ######
  -> avant de exit(status)
    - il faut close les fd de data : free_fds(NULL, 0, data->in_out);
    - il faut free pipe_ptr avant de exit(status); free_pipes(pipe_ptr, (tab_size - 1))
*/
void	exec_builtin(t_data *data, int **pipe_ptr, int tab_size)
{
	if (ft_strcmp(data->args.tab[0], "exit") == 0)
		ft_exit();
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
		ft_cd(data->args.tab);
}
