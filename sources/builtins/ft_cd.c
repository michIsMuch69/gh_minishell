/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:30:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/31 15:07:50 by jedusser         ###   ########.fr       */
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
int	ft_cd(t_data *data)
{
    char	cwd[1024];
    char	*new_dir;
    char	*old_cwd;

    new_dir = NULL;

    // Get the current working directory
    old_cwd = getcwd(cwd, sizeof(cwd));
    if (!old_cwd)
    {
        perror("getcwd");
        return 1;
    }

    // Check arguments and get the new directory to change into
    new_dir = check_cd_args(data, new_dir);
    if (!new_dir)
        return 1;

    // Try to change to the new directory
    if (chdir(new_dir) != 0)
    {
        perror("cd");
        if (new_dir && new_dir != data->args.tab[1])
            free(new_dir);
        return 1;
    }

    // Update OLDPWD environment variable
    set_env("OLDPWD", old_cwd, data->env.tab);

    // Update PWD environment variable, handle deleted directory case
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        set_env("PWD", cwd, data->env.tab);
    }
    else
    {
        // If getcwd() fails after directory change, directory might be deleted
        // Try changing to parent directory and attempt to getcwd again
        if (chdir("..") == 0)
        {
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                set_env("PWD", cwd, data->env.tab);
            else
                return (perror("getcwd"), 1);
        }
        else
        {
            perror("cd");
            return 1;
        }
    }

    // Cleanup if new_dir was dynamically allocated
    if (new_dir && new_dir != data->args.tab[1])
        free(new_dir);

    data->exit_status = 0;
    return data->exit_status;
}
