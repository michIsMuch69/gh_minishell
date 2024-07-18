/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 08:09:19 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/18 12:27:29 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "includes.h"

// main/utils.c
void	free_tab(t_table *tab, int start);

// parsing/pre_treatment.c
char	*pre_treatment(char *prompt, int i);

// parsing/tokenizer.c
t_table	tokenizer(char *prompt);

// parsing/struct_filling.c
int		init_struct(t_data **data, t_table *tokens, int start, int data_size);

#endif