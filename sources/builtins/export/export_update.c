/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:25:24 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/30 07:41:25 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_valid_identifier(char *key)
{
	int	i;

	i = 0;
	if (!key || !key[0] || (key[0] >= '0' && key[0] <= '9'))
		return (0);
	while (key[i])
	{
		if (!(key[i] == '_' || ft_isalnum(key[i])))
			return (0);
		i++;
	}
	return (1);
}

int	process_env_var_for_export(t_vars *vars, char *env_var, t_data *data,
		int index)
{
	vars->equal_pos = ft_strchr(env_var, '=');
	if (vars->equal_pos)
	{
		vars->key = ft_substr(env_var, 0, ft_strlen(env_var)
				- ft_strlen(vars->equal_pos));
		if (!vars->key)
			return (1);
		vars->value = ft_strdup(vars->equal_pos + 1);
		if (!vars->value)
			return (free(vars->key), 1);
		vars->new_var = create_quoted_var(vars->key, vars->value);
		if (!vars->new_var)
			return (free_vars(vars), 1);
		free(vars->key);
		free(vars->value);
	}
	else
	{
		vars->new_var = ft_strdup(env_var);
		if (!vars->new_var)
			return (1);
	}
	data->export.tab[index] = vars->new_var;
	return (0);
}

int	update_or_add_to_tab(char *new_var, t_table *table)
{
	int		i;
	char	*key;
	int		key_len;

	key_len = 0;
	while (new_var[key_len] && new_var[key_len] != '=')
		key_len++;
	key = ft_substr(new_var, 0, key_len);
	if (!key)
		return (1);
	i = 0;
	while (i < table->size)
	{
		if (ft_strncmp(table->tab[i], key, key_len) == 0 && (table->tab[i][key_len] == '\0' || table->tab[i][key_len] == '='))
		{
			update_table(table->tab, i, new_var);
			return (free(key), 0);
		}
		i++;
	}
	table->tab = add_to_table(table, new_var);
	return (free(key), 0);
}

int	process_full_entry(t_vars *vars, t_data *data, t_table *export, int i)
{
	int	key_len;

	key_len = ft_strlen(data->args.tab[i]) - ft_strlen(vars->equal_pos);
	vars->key = ft_substr(data->args.tab[i], 0, key_len);
	if (!vars->key)
		return (1);
	vars->value = ft_strdup(vars->equal_pos + 1);
	if (!vars->value)
		return (free(vars->key), 1);
	if (!is_valid_identifier(vars->key))
		return (ft_printf("export: '%s': not a valid identifier\n", data->args.tab[i]), free_vars(vars), 1);
	vars->new_var = create_quoted_var(vars->key, vars->value);
	if (!vars->new_var)
		return (free_vars(vars), 1);
	if (update_or_add_to_tab(vars->new_var, &data->export) != 0)
		return (free_vars(vars), 1);
	free(vars->new_var);
	vars->new_var = create_unquoted_var(vars->key, vars->value);
	if (!vars->new_var)
		return (free_vars(vars), 1);
	if (update_or_add_to_tab(vars->new_var, &data->env) != 0)
		return (free_vars(vars), 1);
	return (free(vars->new_var), free(vars->key), free(vars->value), 0);
}

int	process_uncomplete_entry(t_vars *vars, t_data *data, t_table *export, int i)
{
	vars->key = ft_strdup(data->args.tab[i]);
	if (!vars->key)
		return (1);
	if (!is_valid_identifier(vars->key))
	{
		ft_printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
		return (free_vars(vars), 1);
	}
	vars->new_var = create_var_without_equals(vars->key);
	if (!vars->new_var)
		return (free_vars(vars), 1);
	if (update_or_add_to_tab(vars->new_var, &data->export) != 0)
		return (free_vars(vars), 1);
	return (free_vars(vars), 0);
}
