/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/10 14:04:23 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

//setenv pour cd 
// int maj_env()
// {
	
// }

int ft_cd(char **args, char **env)
{
   	char	cwd[1024];
    char	*home = NULL;
    char	*oldpwd = NULL;
    char	*new_dir = NULL;
	char	*temp;

	temp = getcwd(cwd, sizeof(cwd));
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
    {
        if (ft_getenv("HOME", env, &home) != 0)
            return (ft_putstr_fd("cd: HOME not set\n", 2), -1);
        new_dir = home;
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        if (ft_getenv("OLDPWD", env, &oldpwd) != 0)
            return (ft_putstr_fd("cd: OLDPWD not set\n", 2), -1);
        new_dir = oldpwd;
    }
    //  ft_strncmp;
	else if (ft_strcmp(args[1], "..") == 0)
		new_dir = "..";
    else
        new_dir = args[1];
    if (chdir(new_dir) != 0)
        return (perror("cd"), free(home), free(oldpwd), -1);
	set_env("OLDPWD", temp, env);
	temp = getcwd(cwd, sizeof(cwd) * sizeof(char));
    if (temp != NULL)
	    set_env("PWD", temp, env);
    else
        perror("getcwd");
	free(home);
    free(oldpwd);
    return 0;
}


//copie pour export, copie env.
//buffer static export (t_table)
//si entree non complete, copie unisuqment dans export + error message print_error
//si entree complete : ok pour copie dans env ++ export.
t_table	ft_tabdup(char **envp);

int	ft_export(t_table args, t_table env)
{
    char	**key;
    char	**value;
	int		i;
	static t_table export_tab;	
	
	i = 0;
	if (!args.tab || !env.tab)
		return (0);
	if (!export_tab.tab)
	{
		export_tab = ft_tabdup(env.tab);
		if (!export_tab.tab)
			return (ft_perror("error _. malloc tab\n") -1);
	}
    if (args.size == 1)
        return (ft_env(export_tab.tab));

	// key = get_all_keys();
	// value = get_all_values();
    // while (i < args.size)
	// {
   	// 	key = args.tab[i];
	// 	i++;
	// 	value = args.tab[i];
    //     set_env(key, value, env.tab);
	// 	i++;
	// }
	//fill_env tab
    return 0;
}

int ft_unset(char *var, t_table *env)
{
    int i;
    int j;

    i = 0;
    j = 0;
	printf("My Unset\n");
	
    while (env->tab[i])
    {
        if (ft_strncmp(env->tab[i], var, ft_strlen(var)) == 0)
        {
            free(env->tab[i]);
            j = i;
            while (env->tab[j])
            {
                env->tab[j] = env->tab[j + 1];
                j++;
            }
    		env->tab[j + 1] = NULL;
            env->size--;
            return (0);
        }
        i++;
    }
    return (1);
}

void	ft_exit(t_data *data, int tab_size, int last_status)
{
	int	status;

	printf("My exit\n");
	status = 0;
	if (data->args.tab[1])
	{
		if (is_numeric_str(data->args.tab[1]) == 0)
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			status = 255;
		}
		else
		{
			status = ft_atoi(data->args.tab[1]);
			status = status % 256;
			if (status < 0)
				status += 256;
			printf("Status in ft_exit = %d\n", status);
		}
	}
	else
	{
		printf("Status in ft_exit no agrs = %d\n", status);
		status = last_status;
	}
	free_struct(data, tab_size);
	exit(status);
}

//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
