/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:28:16 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 13:20:15 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_echo(t_data *data)
{
	int	flag;
	int	i;
	char *tmp1;
	
	ft_getenv("HOME", data->env.tab, &tmp1);
	flag = 0;
	i = 1;
	if (data->args.tab[1] && ft_strcmp(data->args.tab[1], "cd") == 0 && ft_strcmp(data->args.tab[2], "~") == 0)
		return (ft_printf("cd %s\n", tmp1), 0);
	free(tmp1);
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
	if (data->args.size > 1)
		return (127);
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
