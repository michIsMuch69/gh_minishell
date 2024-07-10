/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:27:19 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/10 20:26:25 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>

static int  expand_file(t_table *file, char **envp, int last_exit)
{
	int	i_tab;
	int	ret_value;

	i_tab = 0;
	while (i_tab < file->size)
	{
		while (include_char(file->tab[i_tab], '$', 0) != -1 && \
				count_sign(file->tab[i_tab], file->tab[i_tab][0]) < 2)
		{
			ret_value = change_value(&(file->tab[i_tab]), envp, 0);
			if (ret_value == -1)
				return (-1);
			if (ret_value == 1)
			{
				if (cut_str(&(file->tab[i_tab]), 0, 0) == 1)
				{
					file->size = i_tab - 1;
					return (ft_perror("ambiguous redirect\n"), \
							free_tab(file, file->size), 1);
				}
			}
		}
		i_tab++;
	}
	return (0);
}

static int	arg_management(t_table *arg, char **envp, int last_exit)
{
	int	i_tab;
	int	ret_value;

	i_tab = -1;
	while (++i_tab < arg->size)
	{
		if (arg->tab[i_tab][0] != '\'' && \
			include_char(arg->tab[i_tab], '$', 0) != -1)
		{
			ret_value = change_value(&(arg->tab[i_tab]), envp, last_exit);
			while (ret_value)
			{
				if (ret_value == -1)
					return (-1);
				if (ret_value == 1)
				{
					ret_value = cut_str(&(arg->tab[i_tab]), 0, 0);
					if (ret_value == -1)
						return (-1);
				}
				ret_value = change_value(&(arg->tab[i_tab]), envp, last_exit);
			}
		}
	}
	return (0);
}

/*
	* args->tab ->
		// simple quotes -> no changement
		// double quotes	-> change value if present in env
							-> delete $NAME if not present in env

	* inputs :
		// if HEREDOC -> keep the litteral value
		// if $NAME not present in env -> ambiguous redirect -> clean tab
		// if $NAME present in env -> change the value

	* output :
		// if $NAME not present in env -> ambiguous redirect -> clean tab
		// if $NAME present in env -> change the value

	* heredoc file :
		-> change value if present in env
		-> delete $NAME if not present in env
*/
int	expand_management(t_data *data, char **envp, int last_exit)
{
	int	ret_value;

	ret_value = expand_file(&(data->input), envp, last_exit);
	if (ret_value == -1)
		return (-1);
	else if (ret_value == 1)
		return (1);
	ret_value = expand_file(&(data->output), envp, last_exit);
	if (ret_value == -1)
		return (-1);
	else if (ret_value == 1)
		return (1);
	return (arg_management(&(data->args), envp, last_exit));
}
