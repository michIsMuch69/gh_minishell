/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:39:26 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/17 11:37:46 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// int ft_echo(char **args);
// int ft_cd(char **args);
// int ft_export(char **args);
// int ft_unset(char **args);
// int ft_env(char **env);
// int ft_exit(void);

int	ft_exit(void)
{
	printf("exit function executed\n");
	exit(0);
}

int	ft_cd(char **args)
{
	if (args[1] == NULL)
		return (ft_perror("No arguments to cd\n"), -1);
	else
	{
		ft_printf("This is my own cmd CD\n");;
		if (chdir(args[1]) != 0)
			perror("cd");
	}
	return (0);
}
// int	ft_pwd(void)
// {
	
// }
