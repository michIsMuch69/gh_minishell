/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:26:45 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:11:29 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"

// ####### PROTOTYPES ######## //

// main/utils.c
int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);

// parsing/quote_management.c
int		quote_management(t_table args, t_table *tmp);

// ####### PROTOTYPES ######## //

static int	include_char(char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

static int	clean_token(t_table args, t_table *tmp)
{
	if (include_char(args.tab[tmp->size], '\'') || include_char(args.tab[tmp->size], '\"'))
		return (quote_management(args, tmp));
	else
	{
		tmp->tab[tmp->size] = ft_strdup(args.tab[tmp->size]);
		if (!tmp->tab[tmp->size])
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
		ret_value = clean_token(data->args, &tmp);
		if (ret_value == -1)
			return (free_tab(&tmp, 0), -1);
		if (ret_value == 1)
			return (free_tab(&tmp, 0), 1);
		(tmp.size)++;
	}
	free_tab(&(data->args), 0);
	data->args = tmp;
	return (0);
}
