/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/25 13:36:37 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_cd(t_data *data)
{
	char	cwd[1024];
	char	*new_dir;

	new_dir = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd"), 1);
	if (data->args.size > 2)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (!data->args.tab[1] || ft_strcmp(data->args.tab[1], "~") == 0)
	{
		if (ft_getenv("HOME", data->env.tab, &new_dir) != 0)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
	}
	else if (ft_strcmp(data->args.tab[1], "-") == 0)
	{
		if (ft_getenv("OLDPWD", data->env.tab, &new_dir) != 0)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), 1);
		ft_printf("%s\n", new_dir);
	}
	else
		new_dir = data->args.tab[1];
	if (chdir(new_dir) != 0)
		return (perror("cd"), 1);
	set_env("OLDPWD", cwd, data->env.tab);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env("PWD", cwd, data->env.tab);
	else
		return (perror("getcwd"), 1);
	data->exit_status = 0;
	return (data->exit_status);
}

int	ft_unset_env(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < data->args.size)
	{
		j = 0;
		while (data->env.tab[j])
		{
			if (ft_strncmp(data->env.tab[j], data->args.tab[i],
					ft_strlen(data->args.tab[i])) == 0)
			{
				free(data->env.tab[j]);
				k = j;
				while (data->env.tab[k])
				{
					data->env.tab[k] = data->env.tab[k + 1];
					k++;
				}
				data->env.tab[k] = NULL;
				data->env.size--;
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_unset_export(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < data->args.size)
	{
		j = 0;
		while (data->export.tab[j])
		{
			if (ft_strncmp(data->export.tab[j], data->args.tab[i],
					ft_strlen(data->args.tab[i])) == 0)
			{
				free(data->export.tab[j]);
				k = j;
				while (data->export.tab[k])
				{
					data->export.tab[k] = data->export.tab[k + 1];
					k++;
				}
				data->export.tab[k] = NULL;
				data->export.size--;
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
int	ft_unset(t_data *data)
{
	ft_unset_env(data);
	ft_unset_export(data);
	return (0);
}

int	ft_exit(t_data *data, int i, int **fds, int last_fd)
{
	int	arg;

	arg = 0;
	if (data[i].args.size > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	if (data[i].args.size == 2 && data[i].args.tab[1])
	{
		if (is_numeric_str(data[i].args.tab[1]) == 0)
			return (ft_putstr_fd("exit: numeric argument required\n", 2), 2);
		else
		{
			arg = ft_atoi(data[i].args.tab[1]) % 256;
			if (arg < 0)
				arg += 256;
		}
	}

	if (data->tab_size > 1)
	{
		close_pipes(fds, data->tab_size - 1, 0, last_fd); 
		free_pipes(fds, data->tab_size - 1);
	}
    free_struct(data, data[i].tab_size);
	return (exit(arg), 0);
}

//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
