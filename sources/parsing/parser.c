/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:26:45 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 13:20:07 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ###### INCLUDES ######

#include <stdlib.h>
#include "struct.h"
#include "libft.h"

#include <stdio.h>

// ###### INCLUDES ######


// ###### PROTOTYPES ######

void	free_tab(t_table *tab, int start);
void	free_struct(t_data *struc, int tab_size);
char	*quote_management(char *token, char **envp);
size_t	ft_perror(char *err_message);

// ###### PROTOTYPES ######


static char	*clean_token(char *token)
{
	char	*arg;
	size_t	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
			return (quote_management(token, envp));
		i++;
	}
	return (ft_strdup(token));
}

t_table	*token_cleaner(t_table tokens, char **env)
{
	char	**args;
	t_table	*table;

	if (!tokens.tab)
		return (NULL);
	table = ft_calloc(sizeof(t_table), 1);
	if (!table)
		return (ft_perror("error -> token cleaner mem alloc\n"), NULL);
	table->tab = ft_calloc(sizeof(char *), tokens.size + 1);
	if (!table->tab)
		return (free(table), ft_perror("error -> cleaner S_tab mem alloc\n"), NULL);
	table->size = 0;
	while (table->size < tokens.size)
	{
		table->tab[table->size] = clean_token(tokens.tab[table->size]);
		if (!table->tab[table->size])
			return (free_tab(*table), free(table), NULL);
		(table->size)++;
	}
	return (/*free_all(tokens, NULL, 0),*/ table); // free tokens in ft_parse_prompt during prog builting and test
}
