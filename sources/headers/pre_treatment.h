/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_treatment.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:32:27 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/09 10:33:42 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRE_TREATMENT_H
# define PRE_TREATMENT_H

#include <stdlib.h>
#include <libft.h>

// main/utils.c
int		ft_perror(char *err_message);

// parsing/parsing_utils.c
void	clear_buff(char *buffer, int start, int size);
int		filter(int *i_prompt, int i_tmp, char *prompt, char *tmp);
void	skip_spaces(char *str, int *i);
char	*init_treatment(char *prompt, int *i_tmp, int i_prompt);

#endif