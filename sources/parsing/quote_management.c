/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:07:06 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:12:11 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "struct.h"
#include <stdbool.h>

// main/utils.c
int			ft_perror(char *err_message);
// parsing/parsing_utils.c
char		*final_build(char *token, char c);

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

char	*init_new_str(char *token, int nb_char)
{
	char	new;

	if (nb_char % 2)
		return (ft_perror("error -> syntax\n"), NULL);
	if (!nb_char)
		return (ft_strdup(token));
	return (ft_calloc(ft_strlen(token) + 1, sizeof(char)));
}

int	delete_char(char *token, t_table *tmp)
{
	int	flag;
	int	y;
	int	i;

	i = 0;
	y = 0;
	flag = 0;
	while (token[i])
	{
		if (token[i] == '\"' && flag == 0)
			flag = -1;
		else if (token[i] == '\"' && flag == -1)
		{
			flag = 0;
			i++;
		}
		else if (token[i] == '\'' && flag == 0)
			flag = 1;
		else if (token[i] == '\'' && flag == 1)
		{
			flag = 0;
			i++;
		}
		if (token[i] == '\'' && flag == 1)
			i++;
		else if (token[i] == '\"' && flag == -1)
			i++;
		if (!token[i])
			break ;
		tmp->tab[tmp->size][y] = token[i];
		i++;
		y++;
	}
	return (0);
}

int	quote_management(t_table args, t_table *tmp)
{
	if (quoting_count(args.tab[tmp->size], '\'') % 2 == 1 || quoting_count(args.tab[tmp->size], '\"') % 2 == 1)
		return (ft_perror("error -> syntax\n"), -1);
	tmp->tab[tmp->size] = ft_calloc(ft_strlen(args.tab[tmp->size]) + 1, 1);
	if (!tmp->tab[tmp->size])
		return (ft_perror("alloc -> quote_management"), -1);
	return (delete_char(args.tab[tmp->size], tmp));
}
