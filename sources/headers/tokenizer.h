/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:38:05 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 10:58:01 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <libft.h>
# include <struct.h>
# include <stdlib.h>

// main/utils.c
int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);	

// parsing/parsing_utils.c
int		find_end(char *prompt, char c, int *i);

#endif