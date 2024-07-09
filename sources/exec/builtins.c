/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:39:26 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/09 12:47:26 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// int ft_echo(char **args);
// int ft_cd(char **args);
// int ft_export(char **args);
// int ft_unset(char **args);
// int ft_env(char **env);
// int ft_exit(void);

int	is_numeric_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int ft_echo(char **args)
{
    int flag = 0;
    int i = 1;

    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        flag = 1;
        i++;
    }
    while (args[i])
    {
        ft_printf("%s", args[i]);
        i++;
    }
    if (!flag)
        ft_printf("\n");
    return 0;
}

int	ft_env(char **env)
{
	int	i;

	i = 0;
    while (env[i])
	{
        ft_printf("%s\n", env[i]);
		i++;
	}
    return 0;
}

void	ft_exit(char **args, int last_status)
{
	int	status;

	printf("My exit\n");
	status = 0;
	if (args[1])
	{
		if (is_numeric_str(args[1]) == 0)
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			status = 255;
		}
		else
		{
			status = ft_atoi(args[1]);
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
	exit(status);
}

int	ft_pwd(void)
{
	char	cwd[1024];
	char *temp;

	ft_printf("My pwd\n"); //last exit code
	temp = getcwd(cwd, sizeof(cwd));
	if (temp != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		return (1);
	}
}

void	set_env(char *key, char *value, char **env)
{
    int		i;
    int		key_len;
    int		value_len;
    char	*new_value;

	i = 0;
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_value = malloc((key_len + 1 + value_len + 1) * sizeof(char));
    if (new_value == NULL)
        return;
    ft_memset(new_value, 0, (key_len + 1 + value_len + 1) * sizeof(char));

    ft_strcpy(new_value, key);
    printf("new value befor cat : %s\n", new_value);
    ft_strcat(new_value, "=");
    ft_strcat(new_value, value);
    while (env[i])
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
        {
            free(env[i]);
            env[i] = new_value;
    		printf("size of  key set : %ld\n", ft_strlen(key));
            printf("env key set : %s\n", env[i]);
            return ;
        }
        i++;
    }
    env[i] = new_value;
    env[i + 1] = NULL;
    printf("size of  var added : %ld\n", ft_strlen(env[i]));
    printf("env var added : %s\n", env[i]);
}

// int ft_export(char *var, char *new_value, t_table *env)
// {
//     int i;
//     int var_len;
//     char *temp = NULL;
//     //ft_getenv(var, )
//     // ft_getenv ---> store the VALUE of the variable;
//     i = 0;
//     var_len = ft_strlen(var);
//     //si pas de var : creer et realloc le tableau.
//     // sinon : assigner.
//     while (env->tab[i])
//     {
//         if (ft_strncmp(env->tab[i], var, var_len) == 0)
//         {
//             ft_m
//         }
//     }
// }

int ft_unset(char *var, t_table *env)
{
    int i;
    int j;
    int var_len;

    i = 0;
    j = 0;
    var_len =  ft_strlen(var);

    while (env->tab[i])
    {
        if (ft_strncmp(env->tab[i], var, var_len) == 0)
        {
            free(env->tab[i]);
            j = i;
            while (env->tab[j])
            {
                env->tab[j] = env->tab[j + 1];
                j++;
            }
            env->size--;
    		printf("size of  key : %ld\n", ft_strlen(var));
            printf("env var set : %s\n", env->tab[i]);
            return (1);
        }
		else
			return (-1);
        i++;
    }
    env->tab[i + 1] = NULL;
    printf("size of  key added : %ld\n", ft_strlen(env->tab[i]));
    printf("env var added : %s\n", env->tab[i]);
    return (0);
}
//ft_strncmp(var, ft_strlem(var)(+1?), env[i])

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
