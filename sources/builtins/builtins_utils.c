/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/10 13:46:56 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin_child(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "echo") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "env") == 0)
		return (1);
	return (0);
}

int	is_builtin_parent(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "cd") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "exit") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "export") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "unset") == 0)
		return (1);
	return (0);
}

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



void	set_env(char *key, char *value, char **env)
{
	//pur export
    int		i;
    int		key_len;
    int		value_len;
    char	*new_value;

	i = 0;
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_value = ft_calloc((key_len + value_len + 1), sizeof(char));
    if (new_value == NULL)
        return;
    ft_memset(new_value, 0, (key_len + 1 + value_len + 1) * sizeof(char));

    ft_strcpy(new_value, key);
   // printf("new value befor cat : %s\n", new_value);
    ft_strcat(new_value, "=");
    ft_strcat(new_value, value);
    while (env[i])
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
        {
            free(env[i]);
            env[i] = new_value;
    		// printf("size of  key set : %ld\n", ft_strlen(key));
            // printf("env key set : %s\n", env[i]);
            return ;
        }
        i++;
    }
    env[i] = new_value;
    env[i + 1] = NULL;
	free(new_value);
    // printf("size of  var added : %ld\n", ft_strlen(env[i]));
    // printf("env var added : %s\n", env[i]);
}