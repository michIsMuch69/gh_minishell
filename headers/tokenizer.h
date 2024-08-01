/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:38:05 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:01:12 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "includes.h"

// main/utils.c
int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);

// parsing/parsing_utils.c
int		find_end(char *prompt, char c, int *i);

#endif