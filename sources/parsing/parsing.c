/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 07:33:24 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/26 13:44:47 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

// ###### TEMP PROTOTYPES ######

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

// ###### TEMP PROTOTYPES ######

/*
	#### parse_prompt description ####

	* pre_treatment :
		// analyse of the prompt and add spaces between chars separators
	* tokenizer
		// cut prompt as table
		// ignore unused spaces before and after token
	* init_struct
		// alloc t_data table
		// fill all tables in the structure
			-> args
			-> env
			-> input files
			-> output files
*/
int	parse_prompt(char **env, t_data **data)
{
	t_table	tokens;
	int		struc_tab_size;
	int		ret_value;

	if (!(*data)[0].prompt)
		return (1);
	(*data)[0].prompt = pre_treatment((*data)[0].prompt, 0);
	if (!(*data)->prompt)
		return (-1);
	tokens = tokenizer((*data)->prompt);
	if (!tokens.tab)
		return (-1);
	struc_tab_size = init_struct(data, &tokens, 0, 0);
	if (struc_tab_size == -1)
		return (free_tab(&tokens, 0), -1);
	free_tab(&tokens, 0);
	return (struc_tab_size);
}
