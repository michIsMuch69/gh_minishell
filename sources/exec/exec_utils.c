/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:59:06 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 18:04:34 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	clean_struct(t_data *data)
{
	int ret_value;

	ret_value = expand_management(data, data->env.tab);
	if (ret_value == -1)
		return (-1); // crash -> fermer minishell
	if (ret_value == 1)
		return(1); // -> rendre invite de commande
	ret_value = token_cleaner(data);
	if (ret_value == 1)
		return (1); // -> rendre invite de commande
	if (ret_value == -1)
		return (-1); // crash -> fermer minishell
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	wait_all(int tab_size)
{
	int	i;

	i = 0;
	while (i < tab_size)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
}
