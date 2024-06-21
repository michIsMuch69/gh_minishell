/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:07:06 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 13:08:59 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "struct.h"

// main/utils.c
int		ft_perror(char *err_message);
// parsing/parsing_utils.c
char	*final_build(char *token, char c);

static bool	quoting_count(char *token, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (token[i])
	{
		if (token[i] == c)
			count++;
		i++;
	}
	if (count % 2)
		return (1);
	return (0);
}

static int	double_quote(char *token, t_table tmp)
{
	if (quoting_count(token, '"'))
		return (ft_perror("error -> syntax\n"), 1);
	tmp.tab[tmp.size] = final_build(token, '"');
	if (!tmp.tab[tmp.size])
		return (ft_perror("error-> alloc db quotes\n"), -1);
	return (0);
}

static int	simple_quote(char *token, t_table tmp)
{
	if (quoting_count(token, '\''))
		return (ft_perror("error -> syntax\n"), 1);
	tmp.tab[tmp.size] = final_build(token, '\'');
	if (!tmp.tab[tmp.size])
		return (ft_perror("error-> alloc simple quotes\n"), -1);
	return (0);
}

int	quote_management(t_table args, t_table tmp)
{
	int	i;

	i = 0;
	while (args.tab[tmp.size][i])
	{
		if (args.tab[tmp.size][i] == '\'')
			return (simple_quote(args.tab[tmp.size], tmp));
		else if (args.tab[tmp.size][i] == '"')
			return (double_quote(args.tab[tmp.size], tmp));
		i++;
	}
	return (0);
}
