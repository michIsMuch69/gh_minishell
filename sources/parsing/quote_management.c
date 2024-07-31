/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:07:06 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/31 13:31:12 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "struct.h"
#include "libft.h"

// main/utils.c
int		ft_perror(char *err_message);
// parsing/parsing_utils.c
char	*final_build(char *token, char c);

static int	quoting_count(char *token, char c)
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
	return (count);
}

char *delete_char(char *token, char c, int nb_char)
{
	int		i;
	int		y;
	char	*new;

	i = 0;
	y = 0;
	new = ft_calloc(ft_strlen(token) - nb_char + 1, sizeof(char));
	if (!new)
		return (ft_perror("error -> alloc delete char\n"), NULL);
	while (token[i])
	{
		if (token[i] != c)
		{
			new[y] = token[i];
			y++;
		}
		i++;
	}
	return (new);
}

int	quote_management(t_table args, t_table *tmp)
{
	int		count;
	char	*temp;

	count = quoting_count(args.tab[tmp->size], '\'');
	if (count % 2)
		return (ft_perror("error -> syntax\n"), 1);
	if (count > 0)
	{
		temp = delete_char(args.tab[tmp->size], '\'', count);
		if (!temp)
			return (1);
	}
	count = quoting_count(temp, '\"');
	if (count % 2)
		return (ft_perror("error -> syntax\n"), free(temp), 1);
	if (count > 0)
	{
		tmp->tab[tmp->size] = delete_char(temp, '\"', count);
		if (!tmp->tab[tmp->size])
			return (free(temp), 1);
		free(temp);
	}
	else
		tmp->tab[tmp->size] = temp;
	return (0);
}
