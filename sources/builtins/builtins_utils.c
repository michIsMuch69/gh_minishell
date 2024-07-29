/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/29 12:04:48 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(t_data *data)
{
	if (ft_strcmp(data->args.tab[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "echo") == 0)
		return (1);
	if (ft_strcmp(data->args.tab[0], "env") == 0)
		return (1);
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

void	format_new_env_var(char *key, char *value, char *new_value)
{
	ft_strcpy(new_value, key);
	ft_strcat(new_value, "=");
	ft_strcat(new_value, value);
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
	new_value = ft_calloc((key_len + value_len + 2), sizeof(char));
	if (new_value == NULL)
		return ;
	format_new_env_var(key, value, new_value);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			free(env[i]);
			env[i] = new_value;
			return ;
		}
		i++;
	}
	env[i] = new_value;
	env[i + 1] = NULL;
	free(new_value);
}

void	sort_tab(t_table *table)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < table->size)
	{
		j = 0;
		while (j < table->size - 1 - i)
		{
			if (ft_strcmp(table->tab[j], table->tab[j + 1]) > 0)
			{
				temp = table->tab[j];
				table->tab[j] = table->tab[j + 1];
				table->tab[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
