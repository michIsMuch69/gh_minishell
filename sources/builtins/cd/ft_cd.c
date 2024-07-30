/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:30:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/30 12:53:53 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "exec.h"
#include "includes.h"

char	*check_cd_args(t_data *data, char *new_dir)
{
	if (data->args.size > 2)
		return (ft_putstr_fd("cd: too many arguments\n", 2), NULL);
	if (!data->args.tab[1] || ft_strcmp(data->args.tab[1], "~") == 0)
	{
		if (ft_getenv("HOME", data->env.tab, &new_dir) != 0)
			return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
	}
	else if (ft_strcmp(data->args.tab[1], "-") == 0)
	{
		if (ft_getenv("OLDPWD", data->env.tab, &new_dir) != 0)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), NULL);
		ft_printf("%s\n", new_dir);
	}
	else
		new_dir = data->args.tab[1];
	return (new_dir);
}

int	ft_cd(t_data *data)
{
	char	cwd[1024];
	char	*new_dir;

	new_dir = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd"), 1);
	new_dir = check_cd_args(data, new_dir);
	if (chdir(new_dir) != 0)
		return (perror("cd"), 1);
	if (new_dir && new_dir != data->args.tab[1])
		free(new_dir);
	set_env("OLDPWD", cwd, data->env.tab);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env("PWD", cwd, data->env.tab);
	else
		return (perror("getcwd"), 1);
	data->exit_status = 0;
	return (data->exit_status);
}
