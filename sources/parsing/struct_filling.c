/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_filling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:41:19 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/30 14:15:38 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"

// main/utils.c
int	ft_perror(char *err_message);

static enum e_rtype	find_type(char *token)
{
	if (token[0] == '|')
		return (PIPE);
	if (token[0] == '<')
		return (INPUT);
	if (token[0] == '>')
		return (OUTPUT);
	else
		return (COMMAND);
}

int	fill_tab(t_table *tab, char *token)
{
	char	**tmp;

	if (!tab->size)
	{
		tab->tab = ft_calloc(2, sizeof(char *));
		if (!tab->tab)
			return (ft_perror("error -> alloc struc arg tab\n"), 1);
	}
	else
	{
		tmp = ft_realloc(tab->tab, (sizeof(char *) * (tab->size + 2)), (sizeof(char *) * (tab->size)));
		if (!tmp)
			return (ft_perror("error -> realloc args table\n"), 1);
		tmp[tab->size + 1] = NULL;
		tab->tab = tmp;
	}
	tab->tab[tab->size] = ft_strdup(token);
	if (!tab->tab[tab->size])
		return (ft_perror("error -> strdup\n"), 1);
	(tab->size)++;
	return (0);
}

static int	fill_struct(t_data *struc, t_table *tokens, int *i_tokens)
{
	if (struc->cmd_type == COMMAND)
	{
		if (fill_tab(&(struc->args), tokens->tab[*i_tokens]))
			return (1);
	}
	else if (struc->cmd_type == INPUT)
	{
		if (fill_tab(&(struc->input), tokens->tab[*i_tokens]))
			return (1);
	}
	else if (struc->cmd_type == OUTPUT)
	{
		if (fill_tab(&(struc->output), tokens->tab[*i_tokens]))
			return (1);
	}
	(*i_tokens)++;
	return (0);
}

int	init_struct(t_data **data, t_table *tokens, int i_tokens, int i_data)
{
	t_data			*tmp;

	while (i_tokens < tokens->size)
	{
		if (find_type(tokens->tab[i_tokens]) == PIPE)
		{
			if (!i_tokens)
				return (ft_perror("unexpected \'|\' token\n"), -1);
			tmp = ft_realloc(*data, (sizeof(t_data) * (i_data + 2)), \
							(sizeof(t_data) * (i_data + 1)));
			if (!tmp)
				return (ft_perror("error-> realloc struc tab\n"), -1);
			tmp[i_data + 1].env = tmp[0].env;
			*data = tmp;
			return (init_struct(data, tokens, ++i_tokens, ++i_data));
		}
		else
		{
			(*data)[i_data].cmd_type = find_type(tokens->tab[i_tokens]);
			if (fill_struct(&(*data)[i_data], tokens, &i_tokens) == 1)
				return (-1);
		}
	}
	return (i_data + 1);
}
