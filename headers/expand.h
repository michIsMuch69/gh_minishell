/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:23:46 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/10 20:18:03 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

#include <struct.h>
#include <libft.h>

// main/utils.c
int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);

// parsing/expand_utils.c
int		count_sign(char *str, char sign);
int		change_value(char **token, char **envp, int last_exit);
int		cut_str(char **token, int start, int end);

// parsing/parsing_utils.c
int		include_char(char *token, char c, int start);

// parsing/parsing_utils_nd
int		join_str(char **token, int start, int end, char *var_content);

// parsing/getenv.c
int		ft_getenv(char *word, char **env, char **var_content);

#endif
