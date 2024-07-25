/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/25 08:54:10 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_cd(t_data *data)
{
	char	cwd[1024];
	char	*new_dir;

	new_dir = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		data->exit_status = 1;
		return (data->exit_status);
	}
	if (!data->args.tab[1] || ft_strcmp(data->args.tab[1], "~") == 0)
	{
		if (ft_getenv("HOME", data->env.tab, &new_dir) != 0)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			data->exit_status = 1;
			return (data->exit_status);
		}
	}
	else if (ft_strcmp(data->args.tab[1], "-") == 0)
	{
		if (ft_getenv("OLDPWD", data->env.tab, &new_dir) != 0)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			data->exit_status = 1;
			return (data->exit_status);
		}
		ft_printf("%s\n", new_dir);
	}
	else
	{
		new_dir = data->args.tab[1];
	}
	if (chdir(new_dir) != 0)
	{
		printf("No such\n");
		data->exit_status = 1;
		perror("cd");
		return (data->exit_status);
	}
	printf("oco\n");
	set_env("OLDPWD", cwd, data->env.tab);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		set_env("PWD", cwd, data->env.tab);
	}
	else
	{
		perror("getcwd");
		data->exit_status = 1;
		return (data->exit_status);
	}
	data->exit_status = 0;
	return (data->exit_status);
}

int	ft_unset(t_data *data)
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

int	ft_exit(t_data *data, int tab_size, int last_status)
{
	int	status;

	status = 0;
	if (data->args.size > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		data->exit_status = 1;
		return (status);
	}
	if (data->args.tab[1])
	{
		if (is_numeric_str(data->args.tab[1]) == 0)
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			status = 2;
		}
		else
		{
			status = ft_atoi(data->args.tab[1]) % 256;
			if (status < 0)
				status += 256;
		}
	}
	else
		status = last_status;
	return (free_struct(data, tab_size), exit(status), 0);
}

//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
