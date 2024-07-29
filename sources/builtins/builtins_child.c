/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:28:16 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 10:51:29 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_echo(t_data *data)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	if (data->args.tab[1] && ft_strcmp(data->args.tab[1], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (data->args.tab[i])
	{
		ft_printf("%s", data->args.tab[i]);
		if (data->args.tab[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!flag)
		ft_printf("\n");
	return (0);
}

int	ft_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env.tab[i])
	{
		ft_printf("%s\n", data->env.tab[i]);
		i++;
	}
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[1024];
	char	*temp;

	temp = getcwd(cwd, sizeof(cwd));
	if (temp != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
		return (perror("pwd "), 1);
}
