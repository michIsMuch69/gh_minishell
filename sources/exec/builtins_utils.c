/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 11:20:16 by fberthou         ###   ########.fr       */
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

void	exec_builtin(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "exit") == 0)
		ft_exit();
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
		ft_cd(data->args.tab);
}