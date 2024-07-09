/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 08:09:19 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 10:57:38 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <struct.h>

// main/utils.c
void	free_tab(t_table *tab, int start);

// parsing/pre_treatment.c
char	*pre_treatment(char *prompt, int i);

// parsing/tokenizer.c
t_table	tokenizer(char *prompt);

// parsing/struct_filling.c
int		init_struct(t_data **data, t_table *tokens, int start, int data_size);

#endif