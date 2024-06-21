/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:26:45 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 11:48:10 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"

// main/utils.c
int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);

// parsing/quote_management.c
int		quote_management(t_table args, t_table tmp);

static int	clean_token(t_table args, t_table tmp)
{
	if (args.tab[tmp.size][0] == '\'' || args.tab[tmp.size][0] == '"')
		return (quote_management(args, tmp));
	else
	{
		tmp.tab[tmp.size] = ft_strdup(args.tab[tmp.size]);
		if (!tmp.tab[tmp.size])
			return (-1);
	}
	return (0);
}

int	token_cleaner(t_data *data)
{
	int		ret_value;
	t_table	tmp;

	tmp.tab = ft_calloc(sizeof(char *), data->args.size + 1);
	if (!tmp.tab)
		return (ft_perror("error-> cleaner tab alloc\n"), -1);
	tmp.size = 0;
	while (tmp.size < data->args.size)
	{
		ret_value = clean_token(data->args, tmp);
		if (ret_value == -1)
			return (free_tab(&tmp, 0), -1); // crash alloc have to free data->args
		if (ret_value == 1)
			return (free_tab(&tmp, 0), 1); // syntax error
		(tmp.size)++;
	}
	free_tab(&(data->args), 0);
	data->args = tmp;
	return (0);
}
