/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/12 08:19:23 by jean-michel      ###   ########.fr       */
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



void init_exported_env(char **env, int env_size)
{
    int i;

    g_exported_env = malloc((env_size + 1) * sizeof(char *));
    if (!g_exported_env)
        return;

    for (i = 0; i < env_size; i++)
    {
        g_exported_env[i] = ft_strdup(env[i]);
    }
    g_exported_env[env_size] = NULL;
    g_exported_env_size = env_size;
}

char **add_to_exported(char **exported, int *size, const char *new_var)
{
    char **new_exported;
    int i;

    new_exported = malloc((*size + 2) * sizeof(char *));
    if (!new_exported)
        return exported;

    for (i = 0; i < *size; i++)
    {
        new_exported[i] = exported[i];
    }

    new_exported[i] = ft_strdup(new_var);
    if (!new_exported[i])
    {
        free(new_exported);
        return exported;
    }

    new_exported[i + 1] = NULL;

    if (exported)
        free(exported);

    *size += 1;
    return new_exported;
}

char **add_to_env(char **env, int *size, const char *new_var)
{
    char **new_env;
    int i;
	
	i = 0;
    new_env = malloc((*size + 2) * sizeof(char *));
    if (!new_env)
        return env;

    while (i < *size)
    {
        new_env[i] = env[i];
		i++;
    }
    new_env[i] = ft_strdup(new_var);
    if (!new_env[i])
    {
        free(new_env);
        return env;
    }
    new_env[i + 1] = NULL;
    if (env)
        free(env);
    *size += 1;
    return new_env;
}

void update_env(char **env, int i, char *key, char *value)
{
    int len = ft_strlen(key) + ft_strlen(value) + 2;
    free(env[i]);
    env[i] = malloc(len);
    if (!env[i])
        return ;
    ft_strcpy(env[i], key);
    ft_strcat(env[i], "=");
    ft_strcat(env[i], value);
}

void update_exported(char **exported, int i, char *key)
{
    free(exported[i]);
    exported[i] = ft_strdup(key);
}

int is_valid_identifier(char *key)
{
    int i = 0;

    if (!key || !key[0] || (key[0] >= '0' && key[0] <= '9'))
        return 0;

    while (key[i])
    {
        if (!(key[i] == '_' || ft_isalnum(key[i])))
            return 0;
        i++;
    }

    return 1;
}

void update_or_add_to_exported(char *key)
{
    int j;

    for (j = 0; j < g_exported_env_size; j++)
    {
        if (ft_strncmp(g_exported_env[j], key, ft_strlen(key)) == 0 && (g_exported_env[j][ft_strlen(key)] == '\0' || g_exported_env[j][ft_strlen(key)] == '='))
        {
            update_exported(g_exported_env, j, key);
            return;
        }
    }
    g_exported_env = add_to_exported(g_exported_env, &g_exported_env_size, key);
}

void update_or_add_to_env(char *key, char *value, char ***env, int *env_size)
{
    int k;
	char *new_var;
    for (k = 0; k < *env_size; k++)
    {
        if (ft_strncmp((*env)[k], key, ft_strlen(key)) == 0 && (*env)[k][ft_strlen(key)] == '=')
        {
            update_env(*env, k, key, value);
            return;
        }
    }

    new_var = ft_strjoin(key, "=");
	new_var = ft_strjoin(new_var, value);
    *env = add_to_env(*env, env_size, new_var);
    free(new_var);
}

void process_export_arg(char *arg, char ***env, int *env_size)
{
    char *equals_pos;
    char *value;
    char *key;
	
	equals_pos = ft_strchr(arg, '=');
	key = NULL;
	value = NULL;
    if (equals_pos)
    {
        key = ft_substr(arg, 0, equals_pos - arg);
        value = ft_strdup(equals_pos + 1);
    }
    else
        key = ft_strdup(arg);

    if (!is_valid_identifier(key))
    {
        printf("export: '%s': not a valid identifier\n", arg);
        free(key);
        if (value)
            free(value);
        return;
    }
    update_or_add_to_exported(key);
    if (value)
    {
        update_or_add_to_env(key, value, env, env_size);
        free(value);
    }
    free(key);
}

int ft_export(char **args, char ***env, int *env_size)
{
    int i = 1;
    while (args[i])
    {
        process_export_arg(args[i], env, env_size);
        i++;
    }
    return 0;
}

int ft_export_print(void)
{
    int i = 0;
    while (g_exported_env[i])
    {
        printf("declare -x %s\n", g_exported_env[i]);
        i++;
    }
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
